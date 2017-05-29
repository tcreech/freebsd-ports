--- src/afs/FBSD/osi_vnodeops.c.orig	2016-12-08 04:01:51 UTC
+++ src/afs/FBSD/osi_vnodeops.c
@@ -543,16 +543,21 @@ afs_vop_lookup(ap)
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
@@ -781,20 +786,21 @@ afs_vop_read(ap)
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
 
@@ -803,20 +809,40 @@ afs_vop_getpages(struct vop_getpages_arg
 
     vp = ap->a_vp;
     avc = VTOAFS(vp);
+    pages = ap->a_m;
+#ifdef FBSD_VOP_GETPAGES_BUSIED
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
      * can only occur at the file EOF.
      */
-
     {
-	vm_page_t m = ap->a_m[ap->a_reqpage];
-
+#ifdef FBSD_VOP_GETPAGES_BUSIED
+	AFS_VM_OBJECT_WLOCK(object);
+	ma_vm_page_lock_queues();
+	if(pages[npages - 1]->valid != 0) {
+	    if (--npages == 0) {
+		ma_vm_page_unlock_queues();
+		AFS_VM_OBJECT_WUNLOCK(object);
+		return (VM_PAGER_OK);
+	    }
+	}
+#else
+	vm_page_t m = pages[ap->a_reqpage];
 	AFS_VM_OBJECT_WLOCK(object);
 	ma_vm_page_lock_queues();
 	if (m->valid != 0) {
@@ -824,31 +850,37 @@ afs_vop_getpages(struct vop_getpages_arg
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
+    pmap_qenter(kva, pages, npages);
     MA_PCPU_INC(cnt.v_vnodein);
     MA_PCPU_ADD(cnt.v_vnodepgsin, npages);
 
+#ifdef FBSD_VOP_GETPAGES_BUSIED
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
@@ -861,25 +893,27 @@ afs_vop_getpages(struct vop_getpages_arg
 
     relpbuf(bp, &afs_pbuf_freecnt);
 
-    if (code && (uio.uio_resid == ap->a_count)) {
+    if (code && (uio.uio_resid == count)) {
+#ifndef FBSD_VOP_GETPAGES_BUSIED
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
@@ -903,6 +937,7 @@ afs_vop_getpages(struct vop_getpages_arg
 	    KASSERT(m->dirty == 0, ("afs_getpages: page %p is dirty", m));
 	}
 
+#ifndef FBSD_VOP_GETPAGES_BUSIED
 	if (i != ap->a_reqpage) {
 #if __FreeBSD_version >= 1000042
 	    vm_page_readahead_finish(m);
@@ -942,10 +977,11 @@ afs_vop_getpages(struct vop_getpages_arg
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
