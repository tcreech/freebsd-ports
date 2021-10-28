--- src/afs/FBSD/osi_vnodeops.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/FBSD/osi_vnodeops.c
@@ -61,32 +61,37 @@
 #include <vm/vm_object.h>
 #include <vm/vm_pager.h>
 #include <vm/vnode_pager.h>
+#include <sys/vmmeter.h>
+
+#if defined(AFS_FBSD_UMA_BUFS)
+extern uma_zone_t afs_pbuf_zone;
+#else
 extern int afs_pbuf_freecnt;
+#endif
 
 #define GETNAME()       \
     struct componentname *cnp = ap->a_cnp; \
     char *name; \
-    MALLOC(name, char *, cnp->cn_namelen+1, M_TEMP, M_WAITOK); \
+    name = malloc(cnp->cn_namelen+1, M_TEMP, M_WAITOK); \
     memcpy(name, cnp->cn_nameptr, cnp->cn_namelen); \
     name[cnp->cn_namelen] = '\0'
 
-#define DROPNAME() FREE(name, M_TEMP)
+#define DROPNAME() free(name, M_TEMP)
 
+#ifdef LINK_MAX
+# define AFS_LINK_MAX LINK_MAX
+#else
+# define AFS_LINK_MAX (32767)
+#endif
+
 /*
  * Here we define compatibility functions/macros for interfaces that
  * have changed between different FreeBSD versions.
  */
-#if defined(AFS_FBSD90_ENV)
 static __inline void ma_vm_page_lock_queues(void) {};
 static __inline void ma_vm_page_unlock_queues(void) {};
 static __inline void ma_vm_page_lock(vm_page_t m) { vm_page_lock(m); };
 static __inline void ma_vm_page_unlock(vm_page_t m) { vm_page_unlock(m); };
-#else
-static __inline void ma_vm_page_lock_queues(void) { vm_page_lock_queues(); };
-static __inline void ma_vm_page_unlock_queues(void) { vm_page_unlock_queues(); };
-static __inline void ma_vm_page_lock(vm_page_t m) {};
-static __inline void ma_vm_page_unlock(vm_page_t m) {};
-#endif
 
 #if __FreeBSD_version >= 1000030
 #define AFS_VM_OBJECT_WLOCK(o)	VM_OBJECT_WLOCK(o)
@@ -96,6 +101,14 @@ static __inline void ma_vm_page_unlock(vm_page_t m) {}
 #define AFS_VM_OBJECT_WUNLOCK(o)	VM_OBJECT_UNLOCK(o)
 #endif
 
+#ifdef VM_CNT_ADD
+# define AFS_VM_CNT_ADD(var, x) VM_CNT_ADD(var, x)
+# define AFS_VM_CNT_INC(var)    VM_CNT_INC(var)
+#else
+# define AFS_VM_CNT_ADD(var, x) PCPU_ADD(cnt.var, x)
+# define AFS_VM_CNT_INC(var)    PCPU_INC(cnt.var)
+#endif
+
 /*
  * Mosty copied from sys/ufs/ufs/ufs_vnops.c:ufs_pathconf().
  * We should know the correct answers to these questions with
@@ -110,7 +123,7 @@ afs_vop_pathconf(struct vop_pathconf_args *ap)
 	error = 0;
 	switch (ap->a_name) {
 	case _PC_LINK_MAX:
-		*ap->a_retval = LINK_MAX;
+		*ap->a_retval = AFS_LINK_MAX;
 		break;
 	case _PC_NAME_MAX:
 		*ap->a_retval = NAME_MAX;
@@ -236,7 +249,11 @@ afs_vop_lookup(ap)
      */
 
     if (flags & ISDOTDOT)
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(dvp);
+#else
 	VOP_UNLOCK(dvp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
 
     AFS_GLOCK();
     error = afs_lookup(VTOAFS(dvp), name, &vcp, cnp->cn_cred);
@@ -356,14 +373,14 @@ afs_vop_close(ap)
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
@@ -378,6 +395,14 @@ afs_vop_close(ap)
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
@@ -530,12 +555,16 @@ afs_vop_getpages(struct vop_getpages_args *ap)
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
-    PCPU_INC(cnt.v_vnodein);
-    PCPU_ADD(cnt.v_vnodepgsin, npages);
+    AFS_VM_CNT_INC(v_vnodein);
+    AFS_VM_CNT_ADD(v_vnodepgsin, npages);
 
 #ifdef FBSD_VOP_GETPAGES_BUSIED
     count = ctob(npages);
@@ -558,7 +587,11 @@ afs_vop_getpages(struct vop_getpages_args *ap)
     AFS_GUNLOCK();
     pmap_qremove(kva, npages);
 
+#if defined(AFS_FBSD_UMA_BUFS)
+    uma_zfree(afs_pbuf_zone, bp);
+#else
     relpbuf(bp, &afs_pbuf_freecnt);
+#endif
 
     if (code && (uio.uio_resid == count)) {
 #ifndef FBSD_VOP_GETPAGES_BUSIED
@@ -654,11 +687,19 @@ afs_vop_write(ap)
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
 
@@ -687,6 +728,7 @@ afs_vop_putpages(struct vop_putpages_args *ap)
     vm_offset_t kva;
     struct vnode *vp;
     struct vcache *avc;
+    struct ucred *cred;
 
     memset(&uio, 0, sizeof(uio));
     memset(&iov, 0, sizeof(iov));
@@ -705,12 +747,16 @@ afs_vop_putpages(struct vop_putpages_args *ap)
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
-    PCPU_INC(cnt.v_vnodeout);
-    PCPU_ADD(cnt.v_vnodepgsout, ap->a_count);
+    AFS_VM_CNT_INC(v_vnodeout);
+    AFS_VM_CNT_ADD(v_vnodepgsout, ap->a_count);
 
     iov.iov_base = (caddr_t) kva;
     iov.iov_len = ap->a_count;
@@ -728,19 +774,41 @@ afs_vop_putpages(struct vop_putpages_args *ap)
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
 
@@ -840,7 +908,11 @@ afs_vop_link(ap)
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
@@ -927,10 +999,10 @@ afs_vop_rename(ap)
     if ((error = vn_lock(fvp, LK_EXCLUSIVE)) != 0)
 	goto abortit;
 
-    MALLOC(fname, char *, fcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
+    fname = malloc(fcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
     memcpy(fname, fcnp->cn_nameptr, fcnp->cn_namelen);
     fname[fcnp->cn_namelen] = '\0';
-    MALLOC(tname, char *, tcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
+    tname = malloc(tcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
     memcpy(tname, tcnp->cn_nameptr, tcnp->cn_namelen);
     tname[tcnp->cn_namelen] = '\0';
 
@@ -941,8 +1013,8 @@ afs_vop_rename(ap)
 	afs_rename(VTOAFS(fdvp), fname, VTOAFS(tdvp), tname, tcnp->cn_cred);
     AFS_GUNLOCK();
 
-    FREE(fname, M_TEMP);
-    FREE(tname, M_TEMP);
+    free(fname, M_TEMP);
+    free(tname, M_TEMP);
     if (tdvp == tvp)
 	vrele(tdvp);
     else
@@ -1030,17 +1102,18 @@ afs_vop_symlink(struct vop_symlink_args *ap)
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
@@ -1082,7 +1155,7 @@ afs_vop_readdir(ap)
 	     dp = (const struct dirent *)((const char *)dp + dp->d_reclen))
 	    ncookies++;
 
-	MALLOC(cookies, u_long *, ncookies * sizeof(u_long), M_TEMP,
+	cookies = malloc(ncookies * sizeof(u_long), M_TEMP,
 	       M_WAITOK);
 	for (dp = dp_start, cookiep = cookies; dp < dp_end;
 	     dp = (const struct dirent *)((const char *)dp + dp->d_reclen)) {
@@ -1135,18 +1208,37 @@ afs_vop_inactive(ap)
 static int
 afs_vop_reclaim(struct vop_reclaim_args *ap)
 {
-    /* copied from ../OBSD/osi_vnodeops.c:afs_nbsd_reclaim() */
     int code, slept;
     struct vnode *vp = ap->a_vp;
     struct vcache *avc = VTOAFS(vp);
     int haveGlock = ISAFS_GLOCK();
-    int haveVlock = CheckLock(&afs_xvcache);
 
+    /*
+     * In other code paths, we acquire the vnode lock while afs_xvcache is
+     * already held (e.g. afs_PutVCache() -> vrele()). Here, we already have
+     * the vnode lock, and we need afs_xvcache. So drop the vnode lock in order
+     * to hold afs_xvcache.
+     */
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(vp);
+#else
+    VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
+
     if (!haveGlock)
 	AFS_GLOCK();
-    if (!haveVlock)
-	ObtainWriteLock(&afs_xvcache, 901);
-    /* reclaim the vnode and the in-memory vcache, but keep the on-disk vcache */
+    ObtainWriteLock(&afs_xvcache, 901);
+
+    /*
+     * Note that we deliberately call VOP_LOCK() instead of vn_lock() here.
+     * vn_lock() will return an error for VI_DOOMED vnodes, but we know this
+     * vnode is already VI_DOOMED. We just want to lock it again, and skip the
+     * VI_DOOMED check.
+     */
+    AFS_GUNLOCK();
+    VOP_LOCK(vp, LK_EXCLUSIVE);
+    AFS_GLOCK();
+
     code = afs_FlushVCache(avc, &slept);
 
     if (avc->f.states & CVInit) {
@@ -1154,17 +1246,16 @@ afs_vop_reclaim(struct vop_reclaim_args *ap)
 	afs_osi_Wakeup(&avc->f.states);
     }
 
-    if (!haveVlock)
-	ReleaseWriteLock(&afs_xvcache);
+    ReleaseWriteLock(&afs_xvcache);
     if (!haveGlock)
 	AFS_GUNLOCK();
 
     if (code) {
 	afs_warn("afs_vop_reclaim: afs_FlushVCache failed code %d vnode\n", code);
 	VOP_PRINT(vp);
+	panic("afs: afs_FlushVCache failed during reclaim");
     }
 
-    /* basically, it must not fail */
     vnode_destroy_vobject(vp);
     vp->v_data = 0;
 
@@ -1194,7 +1285,7 @@ afs_vop_print(ap)
     struct vcache *vc = VTOAFS(ap->a_vp);
     int s = vc->f.states;
 
-    printf("vc %p vp %p tag %s, fid: %d.%d.%d.%d, opens %d, writers %d", vc, vp, vp->v_tag,
+    printf("vc %p vp %p, fid: %d.%d.%d.%d, opens %d, writers %d", vc, vp,
 	   (int)vc->f.fid.Cell, (u_int) vc->f.fid.Fid.Volume,
 	   (u_int) vc->f.fid.Fid.Vnode, (u_int) vc->f.fid.Fid.Unique, vc->opens,
 	   vc->execsOrWriters);
@@ -1275,3 +1366,7 @@ struct vop_vector afs_vnodeops = {
 	.vop_symlink =		afs_vop_symlink,
 	.vop_write =		afs_vop_write,
 };
+
+#if defined(AFS_FBSD_HAS_VOP_VECTOR_REGISTER)
+VFS_VOP_VECTOR_REGISTER(afs_vnodeops);
+#endif /* AFS_FBSD_HAS_VOP_VECTOR_REGISTER */
