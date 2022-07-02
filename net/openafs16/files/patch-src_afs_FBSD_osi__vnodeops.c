--- src/afs/FBSD/osi_vnodeops.c.orig	2018-12-13 11:08:47 UTC
+++ src/afs/FBSD/osi_vnodeops.c
@@ -61,6 +61,9 @@
 #include <vm/vm_object.h>
 #include <vm/vm_pager.h>
 #include <vm/vnode_pager.h>
+#if defined(AFS_FBSD120_ENV)
+#include <sys/vmmeter.h>
+#endif
 extern int afs_pbuf_freecnt;
 
 #ifdef AFS_FBSD60_ENV
@@ -229,11 +232,11 @@ struct vnodeopv_desc afs_vnodeop_opv_desc =
 #define GETNAME()       \
     struct componentname *cnp = ap->a_cnp; \
     char *name; \
-    MALLOC(name, char *, cnp->cn_namelen+1, M_TEMP, M_WAITOK); \
+    name =  malloc(cnp->cn_namelen+1, M_TEMP, M_WAITOK); \
     memcpy(name, cnp->cn_nameptr, cnp->cn_namelen); \
     name[cnp->cn_namelen] = '\0'
 
-#define DROPNAME() FREE(name, M_TEMP)
+#define DROPNAME() free(name, M_TEMP)
 
 /*
  * Here we define compatibility functions/macros for interfaces that
@@ -261,12 +264,15 @@ static __inline void ma_vm_page_unlock(vm_page_t m) {}
 #define MA_VOP_UNLOCK(vp, flags, p) (VOP_UNLOCK(vp, flags, p))
 #endif
 
-#if defined(AFS_FBSD70_ENV)
-#define MA_PCPU_INC(c) PCPU_INC(c)
-#define	MA_PCPU_ADD(c, n) PCPU_ADD(c, n)
+#if defined(AFS_FBSD120_ENV)
+#define MA_PCPU_INC(c) VM_CNT_INC(c)
+#define	MA_PCPU_ADD(c, n) VM_CNT_ADD(c, n)
+#elif defined(AFS_FBSD70_ENV)
+#define MA_PCPU_INC(c) PCPU_INC(cnt.c)
+#define	MA_PCPU_ADD(c, n) PCPU_ADD(cnt.c, n)
 #else
-#define MA_PCPU_INC(c) PCPU_LAZY_INC(c)
-#define	MA_PCPU_ADD(c, n) (c) += (n)
+#define MA_PCPU_INC(c) PCPU_LAZY_INC(cnt.##c)
+#define	MA_PCPU_ADD(c, n) (cnt.##c) += (n)
 #endif
 
 #if __FreeBSD_version >= 1000030
@@ -339,17 +345,12 @@ afs_vop_unlock(ap)
     int code = 0;
     u_int op;
     op = ((ap->a_flags) | LK_RELEASE) & LK_TYPE_MASK;
-    int glocked = ISAFS_GLOCK();
-    if (glocked)
-	AFS_GUNLOCK();
     if ((op & (op - 1)) != 0) {
       afs_warn("afs_vop_unlock: Shit.\n");
       goto done;
     }
     code = lockmgr(lkp, ap->a_flags | LK_RELEASE, VI_MTX(vp));
  done:
-    if (glocked)
-	AFS_GLOCK();
     return(code);
 #else
     /* possibly in current code path where this
@@ -398,7 +399,7 @@ afs_vop_pathconf(struct vop_pathconf_args *ap)
 	error = 0;
 	switch (ap->a_name) {
 	case _PC_LINK_MAX:
-		*ap->a_retval = LINK_MAX;
+		*ap->a_retval = 32767;
 		break;
 	case _PC_NAME_MAX:
 		*ap->a_retval = NAME_MAX;
@@ -546,16 +547,21 @@ afs_vop_lookup(ap)
 	ma_vn_lock(vp, LK_EXCLUSIVE | LK_RETRY, p);
 	ma_vn_lock(dvp, LK_EXCLUSIVE | LK_RETRY, p);
 	/* always return the child locked */
+#ifndef AFS_FBSD70_ENV
 	if (lockparent && (flags & ISLASTCN)
 	    && (error = ma_vn_lock(dvp, LK_EXCLUSIVE, p))) {
 	    vput(vp);
 	    DROPNAME();
 	    return (error);
 	}
-    } else if (vp == dvp) {
-	/* they're the same; afs_lookup() already ref'ed the leaf.
-	 * It came in locked, so we don't need to ref OR lock it */
-    } else {
+#endif
+    } else if (vp != dvp) {
+	/* If they were the same, afs_lookup() already ref'ed the leaf.  It
+	 * came in locked, so we didn't need to ref OR lock it.  Otherwise,
+	 * lock dvp and vp according to flags. */
+
+	/* For older FreeBSD, leave the parent locked if
+	 * both LOCKPARENT and ISLASTCN. */
 	if (!lockparent || !(flags & ISLASTCN)) {
 #ifndef AFS_FBSD70_ENV /* 6 too? */
 	    MA_VOP_UNLOCK(dvp, 0, p);	/* done with parent. */
@@ -784,20 +790,21 @@ afs_vop_read(ap)
  *	struct vnode *a_vp;
  *	vm_page_t *a_m;
  *	int a_count;
- *	int a_reqpage;
- *	vm_oofset_t a_offset;
+ *	int *a_rbehind;
+ *	int *a_rahead;
  * };
  */
 int
 afs_vop_getpages(struct vop_getpages_args *ap)
 {
     int code;
-    int i, nextoff, size, toff, npages;
+    int i, nextoff, size, toff, npages, count;
     struct uio uio;
     struct iovec iov;
     struct buf *bp;
     vm_offset_t kva;
     vm_object_t object;
+    vm_page_t *pages;
     struct vnode *vp;
     struct vcache *avc;
 
@@ -806,11 +813,22 @@ afs_vop_getpages(struct vop_getpages_args *ap)
 
     vp = ap->a_vp;
     avc = VTOAFS(vp);
+    pages = ap->a_m;
+#if __FreeBSD_version >= 1100092
+    npages = ap->a_count;
+    if (ap->a_rbehind)
+        *ap->a_rbehind = 0;
+    if (ap->a_rahead)
+        *ap->a_rahead = 0;
+#else
+    npages = btoc(ap->a_count);
+#endif
+
     if ((object = vp->v_object) == NULL) {
 	printf("afs_getpages: called with non-merged cache vnode??\n");
 	return VM_PAGER_ERROR;
     }
-    npages = btoc(ap->a_count);
+
     /*
      * If the requested page is partially valid, just return it and
      * allow the pager to zero-out the blanks.  Partially valid pages
@@ -818,40 +836,57 @@ afs_vop_getpages(struct vop_getpages_args *ap)
      */
 
     {
-	vm_page_t m = ap->a_m[ap->a_reqpage];
 
+#if __FreeBSD_version >= 1100092
 	AFS_VM_OBJECT_WLOCK(object);
 	ma_vm_page_lock_queues();
+	if(pages[npages - 1]->valid != 0) {
+	    if (--npages == 0) {
+		ma_vm_page_unlock_queues();
+		AFS_VM_OBJECT_WUNLOCK(object);
+		return (VM_PAGER_OK);
+	    }
+	}
+#else
+	vm_page_t m = pages[ap->a_reqpage];
+	AFS_VM_OBJECT_WLOCK(object);
+	ma_vm_page_lock_queues();
 	if (m->valid != 0) {
 	    /* handled by vm_fault now        */
 	    /* vm_page_zero_invalid(m, TRUE); */
 	    for (i = 0; i < npages; ++i) {
 		if (i != ap->a_reqpage) {
-		    ma_vm_page_lock(ap->a_m[i]);
-		    vm_page_free(ap->a_m[i]);
-		    ma_vm_page_unlock(ap->a_m[i]);
+		    ma_vm_page_lock(pages[i]);
+		    vm_page_free(pages[i]);
+		    ma_vm_page_unlock(pages[i]);
 		}
 	    }
 	    ma_vm_page_unlock_queues();
 	    AFS_VM_OBJECT_WUNLOCK(object);
 	    return (0);
 	}
+#endif
 	ma_vm_page_unlock_queues();
 	AFS_VM_OBJECT_WUNLOCK(object);
     }
     bp = getpbuf(&afs_pbuf_freecnt);
 
     kva = (vm_offset_t) bp->b_data;
-    pmap_qenter(kva, ap->a_m, npages);
-    MA_PCPU_INC(cnt.v_vnodein);
-    MA_PCPU_ADD(cnt.v_vnodepgsin, npages);
+    pmap_qenter(kva, pages, npages);
+    MA_PCPU_INC(v_vnodein);
+    MA_PCPU_ADD(v_vnodepgsin, npages);
 
+#if __FreeBSD_version >= 1100092
+    count = ctob(npages);
+#else
+    count = ap->a_count;
+#endif
     iov.iov_base = (caddr_t) kva;
-    iov.iov_len = ap->a_count;
+    iov.iov_len = count;
     uio.uio_iov = &iov;
     uio.uio_iovcnt = 1;
-    uio.uio_offset = IDX_TO_OFF(ap->a_m[0]->pindex);
-    uio.uio_resid = ap->a_count;
+    uio.uio_offset = IDX_TO_OFF(pages[0]->pindex);
+    uio.uio_resid = count;
     uio.uio_segflg = UIO_SYSSPACE;
     uio.uio_rw = UIO_READ;
     uio.uio_td = curthread;
@@ -864,25 +899,27 @@ afs_vop_getpages(struct vop_getpages_args *ap)
 
     relpbuf(bp, &afs_pbuf_freecnt);
 
-    if (code && (uio.uio_resid == ap->a_count)) {
+    if (code && (uio.uio_resid == count)) {
+#if __FreeBSD_version < 1100092
 	AFS_VM_OBJECT_WLOCK(object);
 	ma_vm_page_lock_queues();
 	for (i = 0; i < npages; ++i) {
 	    if (i != ap->a_reqpage)
-		vm_page_free(ap->a_m[i]);
+		vm_page_free(pages[i]);
 	}
 	ma_vm_page_unlock_queues();
 	AFS_VM_OBJECT_WUNLOCK(object);
+#endif
 	return VM_PAGER_ERROR;
     }
 
-    size = ap->a_count - uio.uio_resid;
+    size = count - uio.uio_resid;
     AFS_VM_OBJECT_WLOCK(object);
     ma_vm_page_lock_queues();
     for (i = 0, toff = 0; i < npages; i++, toff = nextoff) {
 	vm_page_t m;
 	nextoff = toff + PAGE_SIZE;
-	m = ap->a_m[i];
+	m = pages[i];
 
 	/* XXX not in nfsclient? */
 	m->flags &= ~PG_ZERO;
@@ -906,6 +943,7 @@ afs_vop_getpages(struct vop_getpages_args *ap)
 	    KASSERT(m->dirty == 0, ("afs_getpages: page %p is dirty", m));
 	}
 
+#if __FreeBSD_version < 1100092
 	if (i != ap->a_reqpage) {
 #if __FreeBSD_version >= 1000042
 	    vm_page_readahead_finish(m);
@@ -945,10 +983,11 @@ afs_vop_getpages(struct vop_getpages_args *ap)
 	    }
 #endif	/* __FreeBSD_version 1000042 */
 	}
+#endif   /* ndef FBSD_VOP_GETPAGES_BUSIED */
     }
     ma_vm_page_unlock_queues();
     AFS_VM_OBJECT_WUNLOCK(object);
-    return 0;
+    return VM_PAGER_OK;
 }
 
 int
@@ -1017,8 +1056,8 @@ afs_vop_putpages(struct vop_putpages_args *ap)
 
     kva = (vm_offset_t) bp->b_data;
     pmap_qenter(kva, ap->a_m, npages);
-    MA_PCPU_INC(cnt.v_vnodeout);
-    MA_PCPU_ADD(cnt.v_vnodepgsout, ap->a_count);
+    MA_PCPU_INC(v_vnodeout);
+    MA_PCPU_ADD(v_vnodepgsout, ap->a_count);
 
     iov.iov_base = (caddr_t) kva;
     iov.iov_len = ap->a_count;
@@ -1264,10 +1303,10 @@ afs_vop_rename(ap)
     if ((error = ma_vn_lock(fvp, LK_EXCLUSIVE, p)) != 0)
 	goto abortit;
 
-    MALLOC(fname, char *, fcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
+    fname = malloc(fcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
     memcpy(fname, fcnp->cn_nameptr, fcnp->cn_namelen);
     fname[fcnp->cn_namelen] = '\0';
-    MALLOC(tname, char *, tcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
+    tname = malloc(tcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
     memcpy(tname, tcnp->cn_nameptr, tcnp->cn_namelen);
     tname[tcnp->cn_namelen] = '\0';
 
@@ -1278,8 +1317,8 @@ afs_vop_rename(ap)
 	afs_rename(VTOAFS(fdvp), fname, VTOAFS(tdvp), tname, tcnp->cn_cred);
     AFS_GUNLOCK();
 
-    FREE(fname, M_TEMP);
-    FREE(tname, M_TEMP);
+    free(fname, M_TEMP);
+    free(tname, M_TEMP);
     if (tdvp == tvp)
 	vrele(tdvp);
     else
@@ -1422,8 +1461,7 @@ afs_vop_readdir(ap)
 	     dp = (const struct dirent *)((const char *)dp + dp->d_reclen))
 	    ncookies++;
 
-	MALLOC(cookies, u_long *, ncookies * sizeof(u_long), M_TEMP,
-	       M_WAITOK);
+	cookies = malloc(ncookies * sizeof(u_long), M_TEMP, M_WAITOK);
 	for (dp = dp_start, cookiep = cookies; dp < dp_end;
 	     dp = (const struct dirent *)((const char *)dp + dp->d_reclen)) {
 	    off += dp->d_reclen;
@@ -1589,12 +1627,21 @@ afs_vop_advlock(ap)
 				 * int  a_flags;
 				 * } */ *ap;
 {
-    int error;
+    int error, new_a_op;
     struct ucred cr = *osi_curcred();
 
+    new_a_op = ap->a_op;
+    if(ap->a_op == F_UNLCK){
+	/* This makes no sense; I think passing F_UNLCK rather than
+	 * F_SETLCK in FreeBSD's kern_fcntl is a bug. (We certainly aren't
+	 * being asked to F_SETFD, which happens to equal F_UNLCK.) */
+	/* Quietly alter the erroneous op: */
+	new_a_op = F_SETLK;
+    }
+
     AFS_GLOCK();
     error =
-	afs_lockctl(VTOAFS(ap->a_vp), ap->a_fl, ap->a_op, &cr, (int)ap->a_id);
+	afs_lockctl(VTOAFS(ap->a_vp), ap->a_fl, new_a_op, &cr, (int)ap->a_id);
     AFS_GUNLOCK();
     return error;
 }
