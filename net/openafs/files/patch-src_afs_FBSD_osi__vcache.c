--- src/afs/FBSD/osi_vcache.c.orig	2021-12-09 17:07:41 UTC
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
@@ -91,7 +95,11 @@ osi_AttachVnode(struct vcache *avc, int seq) {
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
@@ -126,7 +134,7 @@ osi_vnhold(struct vcache *avc)
 
     vref(vp);
     VI_LOCK(vp);
-    if ((vp->v_iflag & VI_DOOMED) != 0) {
+    if (VN_IS_DOOMED(vp)) {
 	VI_UNLOCK(vp);
 	vrele(vp);
 	return ENOENT;
