--- src/afs/FBSD/osi_vcache.c.orig	2016-12-08 04:01:51 UTC
+++ src/afs/FBSD/osi_vcache.c
@@ -38,19 +38,20 @@ osi_TryEvictVCache(struct vcache *avc, i
      * This code largely copied from vfs_subr.c:vlrureclaim() */
     vholdl(vp);
 
-    ReleaseWriteLock(&afs_xvcache);
-    AFS_GUNLOCK();
-
     *slept = 1;
     /* use the interlock while locking, so no one else can DOOM this */
-    vn_lock(vp, LK_INTERLOCK|LK_EXCLUSIVE|LK_RETRY);
-    vgone(vp);
-    VOP_UNLOCK(vp, 0);
-    vdrop(vp);
+    /* We won't sleep here since this is a LOR with GLOCK. */
+    if (vn_lock(vp, LK_INTERLOCK|LK_EXCLUSIVE|LK_NOWAIT) == 0) {
+	vgone(vp);
+	VOP_UNLOCK(vp, 0);
+	vdrop(vp);
+	return 1;
+    }
 
-    AFS_GLOCK();
-    ObtainWriteLock(&afs_xvcache, 340);
-    return 1;
+    /* We failed to lock the vnode. Drop and indicate that we weren't able to
+     * flush. Note that vn_lock has dropped the interlock. */
+    vdrop(vp);
+    return 0;
 }
 
 struct vcache *
