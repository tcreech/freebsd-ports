--- src/afs/SOLARIS/osi_vcache.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/SOLARIS/osi_vcache.c
@@ -88,3 +88,19 @@ osi_PostPopulateVCache(struct vcache *avc) {
      * 1 directly. So, we must explicitly VFS_HOLD here. */
     VFS_HOLD(afs_globalVFS);
 }
+
+int
+osi_vnhold(struct vcache *avc)
+{
+    struct vnode *vp = AFSTOV(avc);
+    uint_t prevcount;
+
+    mutex_enter(&vp->v_lock);
+    prevcount = vp->v_count++;
+    mutex_exit(&vp->v_lock);
+
+    if (prevcount == 0) {
+	VFS_HOLD(afs_globalVFS);
+    }
+    return 0;
+}
