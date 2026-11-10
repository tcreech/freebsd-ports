--- src/afs/FBSD/osi_vcache.c.orig	2026-06-25 20:03:19 UTC
+++ src/afs/FBSD/osi_vcache.c
@@ -30,7 +30,7 @@ osi_TryEvictVCache(struct vcache *avc, int *slept, int
 	return evicted;
     }
 
-    if ((vp->v_iflag & VI_DOOMED) != 0) {
+    if (VN_IS_DOOMED(vp)) {
 	VI_UNLOCK(vp);
 	evicted = 1;
 	return evicted;
@@ -53,7 +53,11 @@ osi_TryEvictVCache(struct vcache *avc, int *slept, int
 	    evicted = 1;
 	}
 
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(vp);
+#else
 	VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     }
 
     vdrop(vp);
@@ -97,7 +101,31 @@ osi_AttachVnode(struct vcache *avc, int seq)
     if (!vp->v_mount) {
         vn_lock(vp, LK_EXCLUSIVE | LK_RETRY); /* !glocked */
         insmntque(vp, afs_globalVFS);
+#if __FreeBSD_version >= 1400000
+	/*
+	 * FreeBSD 14 and later split vnode allocation from vnode
+	 * construction: getnewvnode(9) leaves the vnode VSTATE_UNINITIALIZED
+	 * and it is up to the filesystem to complete the transition once the
+	 * vnode has been wired up - compare the vn_set_state(vp,
+	 * VSTATE_CONSTRUCTED) calls in vfs_allocate_syncvnode() and in the
+	 * UFS, msdosfs, tmpfs, ext2fs and nfsclient node allocation paths.
+	 * Without this every AFS vnode stays "uninitialized" as far as the
+	 * VFS layer is concerned, which trips the vnode state assertion in
+	 * the debug VOP wrappers on an INVARIANTS kernel:
+	 *
+	 *     VNASSERT(vn_get_state(vp) != VSTATE_UNINITIALIZED, vp)
+	 *     at sys/kern/vfs_subr.c:5952 (vop_unlock_debugpre)
+	 *     vfs_domount() <- kernel_mount() <- afs_cmount() <- sys_mount()
+	 *
+	 * FreeBSD 13 has no vnode states at all, so it needs nothing here.
+	 */
+	vn_set_state(vp, VSTATE_CONSTRUCTED);
+#endif
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+        VOP_UNLOCK(vp);
+#else
         VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     }
     AFS_GLOCK();
     ObtainWriteLock(&afs_xvcache,339);
@@ -131,9 +159,26 @@ osi_vnhold(struct vcache *avc)
 {
     struct vnode *vp = AFSTOV(avc);
 
+    /*
+     * AFS keeps avc->v without owning a reference to it, so a vnode that is
+     * doomed here may already have been reclaimed - on FreeBSD 13 and later
+     * an idle AFS vnode sits at v_usecount == 0 and v_holdcnt == 0, which
+     * means the kernel owns the storage.  vref() would then update a vnode
+     * that has been recycled, before the check below ever gets to run.
+     *
+     * Look at VI_DOOMED under the interlock first, and keep the existing
+     * check to cover the window between the two.
+     */
+    VI_LOCK(vp);
+    if (VN_IS_DOOMED(vp)) {
+	VI_UNLOCK(vp);
+	return ENOENT;
+    }
+    VI_UNLOCK(vp);
+
     vref(vp);
     VI_LOCK(vp);
-    if ((vp->v_iflag & VI_DOOMED) != 0) {
+    if (VN_IS_DOOMED(vp)) {
 	VI_UNLOCK(vp);
 	vrele(vp);
 	return ENOENT;
