--- src/afs/afs_segments.c.orig	2021-01-14 21:08:41 UTC
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
 #endif /* !AFS_AIX32_ENV && !AFS_SGI65_ENV */
@@ -728,7 +731,12 @@ afs_ExtendSegments(struct vcache *avc, afs_size_t alen
 	    toAdd = AFS_CHUNKTOSIZE(tdc->f.chunk) - offset;
 	}
         tfile = afs_CFileOpen(&tdc->f.inode);
-        osi_Assert(tfile);
+	if (!tfile) {
+	    ReleaseWriteLock(&tdc->lock);
+	    afs_PutDCache(tdc);
+	    code = EIO;
+	    break;
+	}
 	while(tdc->validPos < avc->f.m.Length + toAdd) {
 	     afs_size_t towrite;
 
