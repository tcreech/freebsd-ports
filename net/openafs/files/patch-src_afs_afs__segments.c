--- src/afs/afs_segments.c.orig	2024-10-03 22:32:45 UTC
+++ src/afs/afs_segments.c
@@ -185,7 +185,10 @@ afs_StoreAllSegments(struct vcache *avc, struct vreque
      * on the memcache case since that's we adjust the file's size
      * and finish flushing partial vm pages.
      */
-    if ((cacheDiskType != AFS_FCACHE_TYPE_MEM) ||
+    if ((sync & AFS_NOVMSYNC)) {
+	/* noop */
+
+    } else if ((cacheDiskType != AFS_FCACHE_TYPE_MEM) ||
 	(sync & AFS_VMSYNC_INVAL) || (sync & AFS_VMSYNC) ||
 	(sync & AFS_LASTSTORE))
 #endif /* !AFS_AIX32_ENV && !AFS_SGI_ENV */
