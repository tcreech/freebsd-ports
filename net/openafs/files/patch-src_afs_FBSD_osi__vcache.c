--- src/afs/FBSD/osi_vcache.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/FBSD/osi_vcache.c
@@ -18,39 +18,54 @@ osi_TryEvictVCache(struct vcache *avc, int *slept, int
 {
     struct vnode *vp;
     int code;
+    int evicted = 0;
 
     vp = AFSTOV(avc);
 
     if (!VI_TRYLOCK(vp))
-	return 0;
+	return evicted;
     code = osi_fbsd_checkinuse(avc);
     if (code != 0) {
 	VI_UNLOCK(vp);
-	return 0;
+	return evicted;
     }
 
-    if ((vp->v_iflag & VI_DOOMED) != 0) {
+    if (VN_IS_DOOMED(vp)) {
 	VI_UNLOCK(vp);
-	return 1;
+	evicted = 1;
+	return evicted;
     }
 
-    /* must hold the vnode before calling vgone()
-     * This code largely copied from vfs_subr.c:vlrureclaim() */
     vholdl(vp);
 
     ReleaseWriteLock(&afs_xvcache);
     AFS_GUNLOCK();
 
     *slept = 1;
-    /* use the interlock while locking, so no one else can DOOM this */
-    vn_lock(vp, LK_INTERLOCK|LK_EXCLUSIVE|LK_RETRY);
-    vgone(vp);
-    VOP_UNLOCK(vp, 0);
+
+    if (vn_lock(vp, LK_INTERLOCK|LK_EXCLUSIVE|LK_NOWAIT) == 0) {
+	/*
+	 * vrecycle() will vgone() only if its usecount is 0. If someone grabbed a
+	 * new usecount ref just now, the vgone() will be skipped, and vrecycle
+	 * will return 0.
+	 */
+	if (vrecycle(vp) != 0) {
+	    evicted = 1;
+	}
+
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(vp);
+#else
+	VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
+    }
+
     vdrop(vp);
 
     AFS_GLOCK();
     ObtainWriteLock(&afs_xvcache, 340);
-    return 1;
+
+    return evicted;
 }
 
 struct vcache *
@@ -80,7 +95,11 @@ osi_AttachVnode(struct vcache *avc, int seq) {
     if (!vp->v_mount) {
         vn_lock(vp, LK_EXCLUSIVE | LK_RETRY); /* !glocked */
         insmntque(vp, afs_globalVFS);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+        VOP_UNLOCK(vp);
+#else
         VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     }
     AFS_GLOCK();
     ObtainWriteLock(&afs_xvcache,339);
@@ -108,3 +127,19 @@ osi_PostPopulateVCache(struct vcache *avc) {
     vSetType(avc, VREG);
 }
 
+int
+osi_vnhold(struct vcache *avc)
+{
+    struct vnode *vp = AFSTOV(avc);
+
+    vref(vp);
+    VI_LOCK(vp);
+    if (VN_IS_DOOMED(vp)) {
+	VI_UNLOCK(vp);
+	vrele(vp);
+	return ENOENT;
+    }
+
+    VI_UNLOCK(vp);
+    return 0;
+}
