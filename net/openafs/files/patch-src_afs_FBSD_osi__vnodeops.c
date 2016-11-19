--- src/afs/FBSD/osi_vnodeops.c.orig	2016-11-10 16:56:30 UTC
+++ src/afs/FBSD/osi_vnodeops.c
@@ -79,7 +79,6 @@ static vop_mkdir_t	afs_vop_mkdir;
 static vop_mknod_t	afs_vop_mknod;
 static vop_open_t	afs_vop_open;
 static vop_pathconf_t	afs_vop_pathconf;
-static vop_poll_t	afs_vop_poll;
 static vop_print_t	afs_vop_print;
 static vop_putpages_t	afs_vop_putpages;
 static vop_read_t	afs_vop_read;
@@ -120,7 +119,6 @@ struct vop_vector afs_vnodeops = {
 	.vop_mknod =		afs_vop_mknod,
 	.vop_open =		afs_vop_open,
 	.vop_pathconf =		afs_vop_pathconf,
-	.vop_poll =		afs_vop_poll,
 	.vop_print =		afs_vop_print,
 	.vop_putpages =		afs_vop_putpages,
 	.vop_read =		afs_vop_read,
@@ -157,7 +155,6 @@ int afs_vop_getpages(struct vop_getpages
 int afs_vop_putpages(struct vop_putpages_args *);
 int afs_vop_ioctl(struct vop_ioctl_args *);
 static int afs_vop_pathconf(struct vop_pathconf_args *);
-int afs_vop_poll(struct vop_poll_args *);
 int afs_vop_fsync(struct vop_fsync_args *);
 int afs_vop_remove(struct vop_remove_args *);
 int afs_vop_link(struct vop_link_args *);
@@ -200,7 +197,7 @@ struct vnodeopv_entry_desc afs_vnodeop_e
     {&vop_mknod_desc, (vop_t *) afs_vop_mknod},	/* mknod */
     {&vop_open_desc, (vop_t *) afs_vop_open},	/* open */
     {&vop_pathconf_desc, (vop_t *) afs_vop_pathconf},	/* pathconf */
-    {&vop_poll_desc, (vop_t *) afs_vop_poll},	/* select */
+    {&vop_poll_desc, (vop_t *) vop_nopoll},	/* select */
     {&vop_print_desc, (vop_t *) afs_vop_print},	/* print */
     {&vop_read_desc, (vop_t *) afs_vop_read},	/* read */
     {&vop_readdir_desc, (vop_t *) afs_vop_readdir},	/* readdir */
@@ -592,6 +589,7 @@ afs_vop_create(ap)
     GETNAME();
 
     AFS_GLOCK();
+    ap->a_vap->va_mode |= S_IRUSR;
     error =
 	afs_create(VTOAFS(dvp), name, ap->a_vap,
 		   ap->a_vap->va_vaflags & VA_EXCLUSIVE ? EXCL : NONEXCL,
@@ -784,20 +782,21 @@ afs_vop_read(ap)
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
 
@@ -806,52 +805,80 @@ afs_vop_getpages(struct vop_getpages_arg
 
     vp = ap->a_vp;
     avc = VTOAFS(vp);
+    pages = ap->a_m;
+#ifdef AFS_FBSD110_ENV
+    npages = ap->a_count;
+#else
+    npages = btoc(ap->a_count);
+#endif
+
     if ((object = vp->v_object) == NULL) {
-	printf("afs_getpages: called with non-merged cache vnode??\n");
-	return VM_PAGER_ERROR;
+        printf("afs_getpages: called with non-merged cache vnode??\n");
+        return VM_PAGER_ERROR;
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
+#ifdef AFS_FBSD110_ENV
+        AFS_VM_OBJECT_WLOCK(object);
+        ma_vm_page_lock_queues();
+        if(pages[npages - 1]->valid != 0) {
+            if (--npages == 0) {
+                ma_vm_page_unlock_queues();
+                AFS_VM_OBJECT_WUNLOCK(object);
 
-	AFS_VM_OBJECT_WLOCK(object);
-	ma_vm_page_lock_queues();
-	if (m->valid != 0) {
-	    /* handled by vm_fault now        */
-	    /* vm_page_zero_invalid(m, TRUE); */
-	    for (i = 0; i < npages; ++i) {
-		if (i != ap->a_reqpage) {
-		    ma_vm_page_lock(ap->a_m[i]);
-		    vm_page_free(ap->a_m[i]);
-		    ma_vm_page_unlock(ap->a_m[i]);
-		}
-	    }
-	    ma_vm_page_unlock_queues();
-	    AFS_VM_OBJECT_WUNLOCK(object);
-	    return (0);
-	}
-	ma_vm_page_unlock_queues();
-	AFS_VM_OBJECT_WUNLOCK(object);
+                if (ap->a_rbehind)
+                    *ap->a_rbehind = 0;
+                if (ap->a_rahead)
+                    *ap->a_rahead = 0;
+
+                return (VM_PAGER_OK);
+            }
+        }
+#else
+        vm_page_t m = pages[ap->a_reqpage];
+        AFS_VM_OBJECT_WLOCK(object);
+        ma_vm_page_lock_queues();
+        if (m->valid != 0) {
+            /* handled by vm_fault now        */
+            /* vm_page_zero_invalid(m, TRUE); */
+            for (i = 0; i < npages; ++i) {
+                if (i != ap->a_reqpage) {
+                    ma_vm_page_lock(pages[i]);
+                    vm_page_free(pages[i]);
+                    ma_vm_page_unlock(pages[i]);
+                }
+            }
+            ma_vm_page_unlock_queues();
+            AFS_VM_OBJECT_WUNLOCK(object);
+            return (0);
+        }
+#endif
+        ma_vm_page_unlock_queues();
+        AFS_VM_OBJECT_WUNLOCK(object);
     }
     bp = getpbuf(&afs_pbuf_freecnt);
 
     kva = (vm_offset_t) bp->b_data;
-    pmap_qenter(kva, ap->a_m, npages);
+    pmap_qenter(kva, pages, npages);
     MA_PCPU_INC(cnt.v_vnodein);
     MA_PCPU_ADD(cnt.v_vnodepgsin, npages);
 
+#ifdef AFS_FBSD110_ENV
+    count = npages << PAGE_SHIFT;
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
@@ -864,91 +891,105 @@ afs_vop_getpages(struct vop_getpages_arg
 
     relpbuf(bp, &afs_pbuf_freecnt);
 
-    if (code && (uio.uio_resid == ap->a_count)) {
+    if (code && (uio.uio_resid == count)) {
+#ifndef AFS_FBSD110_ENV
 	AFS_VM_OBJECT_WLOCK(object);
 	ma_vm_page_lock_queues();
 	for (i = 0; i < npages; ++i) {
 	    if (i != ap->a_reqpage)
-		vm_page_free(ap->a_m[i]);
+		vm_page_free(pages[i]);
 	}
 	ma_vm_page_unlock_queues();
 	AFS_VM_OBJECT_WUNLOCK(object);
-	return VM_PAGER_ERROR;
+#endif
+        return VM_PAGER_ERROR;
     }
 
-    size = ap->a_count - uio.uio_resid;
+    size = count - uio.uio_resid;
     AFS_VM_OBJECT_WLOCK(object);
     ma_vm_page_lock_queues();
     for (i = 0, toff = 0; i < npages; i++, toff = nextoff) {
-	vm_page_t m;
-	nextoff = toff + PAGE_SIZE;
-	m = ap->a_m[i];
+        vm_page_t m;
+        nextoff = toff + PAGE_SIZE;
+        m = pages[i];
 
-	/* XXX not in nfsclient? */
-	m->flags &= ~PG_ZERO;
+        /* XXX not in nfsclient? */
+        m->flags &= ~PG_ZERO;
 
-	if (nextoff <= size) {
-	    /*
-	     * Read operation filled an entire page
-	     */
-	    m->valid = VM_PAGE_BITS_ALL;
+        if (nextoff <= size) {
+            /*
+             * Read operation filled an entire page
+             */
+            m->valid = VM_PAGE_BITS_ALL;
 #ifndef AFS_FBSD80_ENV
-	    vm_page_undirty(m);
+            vm_page_undirty(m);
 #else
-	    KASSERT(m->dirty == 0, ("afs_getpages: page %p is dirty", m));
+            KASSERT(m->dirty == 0, ("afs_getpages: page %p is dirty", m));
 #endif
-	} else if (size > toff) {
-	    /*
-	     * Read operation filled a partial page.
-	     */
-	    m->valid = 0;
-	    vm_page_set_validclean(m, 0, size - toff);
-	    KASSERT(m->dirty == 0, ("afs_getpages: page %p is dirty", m));
-	}
+        } else if (size > toff) {
+            /*
+             * Read operation filled a partial page.
+             */
+            m->valid = 0;
+#ifdef AFS_FBSD110_ENV
+            vm_page_set_valid_range(m, 0, size - toff);
+#else
+            vm_page_set_validclean(m, 0, size - toff);
+#endif
+            KASSERT(m->dirty == 0, ("afs_getpages: page %p is dirty", m));
+        }
 
-	if (i != ap->a_reqpage) {
+#ifndef AFS_FBSD110_ENV
+        if (i != ap->a_reqpage) {
 #if __FreeBSD_version >= 1000042
-	    vm_page_readahead_finish(m);
+            vm_page_readahead_finish(m);
 #else
-	    /*
-	     * Whether or not to leave the page activated is up in
-	     * the air, but we should put the page on a page queue
-	     * somewhere (it already is in the object).  Result:
-	     * It appears that emperical results show that
-	     * deactivating pages is best.
-	     */
+            /*
+             * Whether or not to leave the page activated is up in
+             * the air, but we should put the page on a page queue
+             * somewhere (it already is in the object).  Result:
+             * It appears that emperical results show that
+             * deactivating pages is best.
+             */
 
-	    /*
-	     * Just in case someone was asking for this page we
-	     * now tell them that it is ok to use.
-	     */
-	    if (!code) {
+            /*
+             * Just in case someone was asking for this page we
+             * now tell them that it is ok to use.
+             */
+            if (!code) {
 #if defined(AFS_FBSD70_ENV)
-		if (m->oflags & VPO_WANTED) {
+                if (m->oflags & VPO_WANTED) {
 #else
-		if (m->flags & PG_WANTED) {
+                if (m->flags & PG_WANTED) {
 #endif
-		    ma_vm_page_lock(m);
-		    vm_page_activate(m);
-		    ma_vm_page_unlock(m);
-		}
-		else {
-		    ma_vm_page_lock(m);
-		    vm_page_deactivate(m);
-		    ma_vm_page_unlock(m);
-		}
-		vm_page_wakeup(m);
-	    } else {
-		ma_vm_page_lock(m);
-		vm_page_free(m);
-		ma_vm_page_unlock(m);
-	    }
+                    ma_vm_page_lock(m);
+                    vm_page_activate(m);
+                    ma_vm_page_unlock(m);
+                }
+                else {
+                    ma_vm_page_lock(m);
+                    vm_page_deactivate(m);
+                    ma_vm_page_unlock(m);
+                }
+                vm_page_wakeup(m);
+            } else {
+                ma_vm_page_lock(m);
+                vm_page_free(m);
+                ma_vm_page_unlock(m);
+            }
 #endif	/* __FreeBSD_version 1000042 */
-	}
+        }
+#endif   /* ndef AFS_FBSD110_ENV */
     }
     ma_vm_page_unlock_queues();
     AFS_VM_OBJECT_WUNLOCK(object);
-    return 0;
+#ifdef AFS_FBSD110_ENV
+    if (ap->a_rbehind)
+        *ap->a_rbehind = 0;
+    if (ap->a_rahead)
+        *ap->a_rahead = 0;
+#endif
+    return VM_PAGER_OK;
 }
 
 int
@@ -977,7 +1018,6 @@ afs_vop_write(ap)
  *	int a_count;
  *	int a_sync;
  *	int *a_rtvals;
- *	vm_oofset_t a_offset;
  * };
  */
 /*
@@ -1081,22 +1121,6 @@ afs_vop_ioctl(ap)
     }
 }
 
-/* ARGSUSED */
-int
-afs_vop_poll(ap)
-     struct vop_poll_args	/* {
-				 * struct vnode *a_vp;
-				 * int  a_events;
-				 * struct ucred *a_cred;
-				 * struct thread *td;
-				 * } */ *ap;
-{
-    /*
-     * We should really check to see if I/O is possible.
-     */
-    return (1);
-}
-
 int
 afs_vop_fsync(ap)
      struct vop_fsync_args	/* {
@@ -1589,12 +1613,21 @@ afs_vop_advlock(ap)
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
+        /* Quietly alter the erroneous op: */
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
