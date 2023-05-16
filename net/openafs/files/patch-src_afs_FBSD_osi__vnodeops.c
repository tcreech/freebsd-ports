--- src/afs/FBSD/osi_vnodeops.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/FBSD/osi_vnodeops.c
@@ -62,7 +62,12 @@
 #include <vm/vm_pager.h>
 #include <vm/vnode_pager.h>
 #include <sys/vmmeter.h>
+
+#if defined(AFS_FBSD_UMA_BUFS)
+extern uma_zone_t afs_pbuf_zone;
+#else
 extern int afs_pbuf_freecnt;
+#endif
 
 #define GETNAME()       \
     struct componentname *cnp = ap->a_cnp; \
@@ -244,7 +249,11 @@ afs_vop_lookup(ap)
      */
 
     if (flags & ISDOTDOT)
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(dvp);
+#else
 	VOP_UNLOCK(dvp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
 
     AFS_GLOCK();
     error = afs_lookup(VTOAFS(dvp), name, &vcp, cnp->cn_cred);
@@ -364,14 +373,14 @@ afs_vop_close(ap)
 				 * struct thread *a_td;
 				 * } */ *ap;
 {
-    int code, iflag;
+    int code, doomed;
     struct vnode *vp = ap->a_vp;
     struct vcache *avc = VTOAFS(vp);
 
     VI_LOCK(vp);
-    iflag = vp->v_iflag & VI_DOOMED;
+    doomed = VN_IS_DOOMED(vp);
     VI_UNLOCK(vp);
-    if (iflag & VI_DOOMED) {
+    if (doomed) {
         /* osi_FlushVCache (correctly) calls vgone() on recycled vnodes, we don't
          * have an afs_close to process, in that case */
         if (avc->opens != 0)
@@ -386,6 +395,14 @@ afs_vop_close(ap)
     else
 	code = afs_close(avc, ap->a_fflag, afs_osi_credp);
     osi_FlushPages(avc, ap->a_cred);	/* hold GLOCK, but not basic vnode lock */
+
+    ObtainWriteLock(&avc->lock, 808);
+    if (avc->cred != NULL) {
+	crfree(avc->cred);
+	avc->cred = NULL;
+    }
+    ReleaseWriteLock(&avc->lock);
+
     AFS_GUNLOCK();
     return code;
 }
@@ -538,7 +555,11 @@ afs_vop_getpages(struct vop_getpages_args *ap)
 	ma_vm_page_unlock_queues();
 	AFS_VM_OBJECT_WUNLOCK(object);
     }
+#if defined(AFS_FBSD_UMA_BUFS)
+    bp = uma_zalloc(afs_pbuf_zone, M_WAITOK);
+#else
     bp = getpbuf(&afs_pbuf_freecnt);
+#endif
 
     kva = (vm_offset_t) bp->b_data;
     pmap_qenter(kva, pages, npages);
@@ -566,7 +587,11 @@ afs_vop_getpages(struct vop_getpages_args *ap)
     AFS_GUNLOCK();
     pmap_qremove(kva, npages);
 
+#if defined(AFS_FBSD_UMA_BUFS)
+    uma_zfree(afs_pbuf_zone, bp);
+#else
     relpbuf(bp, &afs_pbuf_freecnt);
+#endif
 
     if (code && (uio.uio_resid == count)) {
 #ifndef FBSD_VOP_GETPAGES_BUSIED
@@ -662,11 +687,19 @@ afs_vop_write(ap)
 {
     int code;
     struct vcache *avc = VTOAFS(ap->a_vp);
+    off_t start, end;
+    start = AFS_UIO_OFFSET(ap->a_uio);
+    end = start + AFS_UIO_RESID(ap->a_uio);
+
     AFS_GLOCK();
     osi_FlushPages(avc, ap->a_cred);	/* hold GLOCK, but not basic vnode lock */
     code =
 	afs_write(VTOAFS(ap->a_vp), ap->a_uio, ap->a_ioflag, ap->a_cred, 0);
     AFS_GUNLOCK();
+
+    /* Invalidate any pages in the written area. */
+    vn_pages_remove(ap->a_vp, OFF_TO_IDX(start), OFF_TO_IDX(end));
+
     return code;
 }
 
@@ -695,6 +728,7 @@ afs_vop_putpages(struct vop_putpages_args *ap)
     vm_offset_t kva;
     struct vnode *vp;
     struct vcache *avc;
+    struct ucred *cred;
 
     memset(&uio, 0, sizeof(uio));
     memset(&iov, 0, sizeof(iov));
@@ -713,7 +747,11 @@ afs_vop_putpages(struct vop_putpages_args *ap)
     npages = btoc(ap->a_count);
     for (i = 0; i < npages; i++)
 	ap->a_rtvals[i] = VM_PAGER_AGAIN;
+#if defined(AFS_FBSD_UMA_BUFS)
+    bp = uma_zalloc(afs_pbuf_zone, M_WAITOK);
+#else
     bp = getpbuf(&afs_pbuf_freecnt);
+#endif
 
     kva = (vm_offset_t) bp->b_data;
     pmap_qenter(kva, ap->a_m, npages);
@@ -736,19 +774,41 @@ afs_vop_putpages(struct vop_putpages_args *ap)
      * sync |= IO_INVAL; */
 
     AFS_GLOCK();
-    code = afs_write(avc, &uio, sync, osi_curcred(), 0);
+
+    ObtainReadLock(&avc->lock);
+    if (avc->cred != NULL) {
+	/*
+	 * Use the creds from the process that opened this file for writing; if
+	 * any. Otherwise, if we use the current process's creds, we may use
+	 * the creds for uid 0 if we are writing back pages from the syncer(4)
+	 * process.
+	 */
+	cred = crhold(avc->cred);
+    } else {
+	cred = crhold(curthread->td_ucred);
+    }
+    ReleaseReadLock(&avc->lock);
+
+    code = afs_write(avc, &uio, sync, cred, 0);
     AFS_GUNLOCK();
 
     pmap_qremove(kva, npages);
+#if defined(AFS_FBSD_UMA_BUFS)
+    uma_zfree(afs_pbuf_zone, bp);
+#else
     relpbuf(bp, &afs_pbuf_freecnt);
+#endif
 
     if (!code) {
+	AFS_VM_OBJECT_WLOCK(vp->v_object);
 	size = ap->a_count - uio.uio_resid;
 	for (i = 0; i < round_page(size) / PAGE_SIZE; i++) {
 	    ap->a_rtvals[i] = VM_PAGER_OK;
 	    vm_page_undirty(ap->a_m[i]);
 	}
+	AFS_VM_OBJECT_WUNLOCK(vp->v_object);
     }
+    crfree(cred);
     return ap->a_rtvals[0];
 }
 
@@ -848,7 +908,11 @@ afs_vop_link(ap)
     error = afs_link(VTOAFS(vp), VTOAFS(dvp), name, cnp->cn_cred);
     AFS_GUNLOCK();
     if (dvp != vp)
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(vp);
+#else
 	VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
   out:
     DROPNAME();
     return error;
@@ -1038,17 +1102,18 @@ afs_vop_symlink(struct vop_symlink_args *ap)
     dvp = ap->a_dvp;
     newvp = NULL;
 
+    // __DECONST needed as of 1493c2ee62b8
     error =
-	afs_symlink(VTOAFS(dvp), name, ap->a_vap, ap->a_target, NULL,
+	afs_symlink(VTOAFS(dvp), name, ap->a_vap, __DECONST(char*, ap->a_target), NULL,
 		    cnp->cn_cred);
     if (error == 0) {
 	error = afs_lookup(VTOAFS(dvp), name, &vcp, cnp->cn_cred);
-	if (error == 0) {
-	    newvp = AFSTOV(vcp);
-	    vn_lock(newvp, LK_EXCLUSIVE | LK_RETRY);
-	}
     }
     AFS_GUNLOCK();
+    if (error == 0) {
+	newvp = AFSTOV(vcp);
+	vn_lock(newvp, LK_EXCLUSIVE | LK_RETRY);
+    }
     DROPNAME();
     *(ap->a_vpp) = newvp;
     return error;
@@ -1154,7 +1219,11 @@ afs_vop_reclaim(struct vop_reclaim_args *ap)
      * the vnode lock, and we need afs_xvcache. So drop the vnode lock in order
      * to hold afs_xvcache.
      */
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(vp);
+#else
     VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
 
     if (!haveGlock)
 	AFS_GLOCK();
@@ -1166,7 +1235,9 @@ afs_vop_reclaim(struct vop_reclaim_args *ap)
      * vnode is already VI_DOOMED. We just want to lock it again, and skip the
      * VI_DOOMED check.
      */
+    AFS_GUNLOCK();
     VOP_LOCK(vp, LK_EXCLUSIVE);
+    AFS_GLOCK();
 
     code = afs_FlushVCache(avc, &slept);
 
@@ -1214,7 +1285,7 @@ afs_vop_print(ap)
     struct vcache *vc = VTOAFS(ap->a_vp);
     int s = vc->f.states;
 
-    printf("vc %p vp %p tag %s, fid: %d.%d.%d.%d, opens %d, writers %d", vc, vp, vp->v_tag,
+    printf("vc %p vp %p, fid: %d.%d.%d.%d, opens %d, writers %d", vc, vp,
 	   (int)vc->f.fid.Cell, (u_int) vc->f.fid.Fid.Volume,
 	   (u_int) vc->f.fid.Fid.Vnode, (u_int) vc->f.fid.Fid.Unique, vc->opens,
 	   vc->execsOrWriters);
@@ -1241,6 +1312,7 @@ afs_vop_advlock(ap)
 {
     int error, a_op;
     struct ucred cr = *osi_curcred();
+    struct vnode *vp = ap->a_vp;
 
     a_op = ap->a_op;
     if (a_op == F_UNLCK) {
@@ -1254,6 +1326,7 @@ afs_vop_advlock(ap)
 	a_op = F_SETLK;
     }
 
+    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY);
     AFS_GLOCK();
     error =
 	afs_lockctl(VTOAFS(ap->a_vp),
@@ -1261,6 +1334,11 @@ afs_vop_advlock(ap)
 		a_op, &cr,
 		(int)(intptr_t)ap->a_id);	/* XXX: no longer unique! */
     AFS_GUNLOCK();
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(vp);
+#else
+	VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     return error;
 }
 
@@ -1295,3 +1373,7 @@ struct vop_vector afs_vnodeops = {
 	.vop_symlink =		afs_vop_symlink,
 	.vop_write =		afs_vop_write,
 };
+
+#if defined(AFS_FBSD_HAS_VOP_VECTOR_REGISTER)
+VFS_VOP_VECTOR_REGISTER(afs_vnodeops);
+#endif /* AFS_FBSD_HAS_VOP_VECTOR_REGISTER */
