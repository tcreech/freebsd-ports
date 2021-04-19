--- src/afs/afs.h.orig	2016-12-08 04:01:51 UTC
+++ src/afs/afs.h
@@ -1609,8 +1609,10 @@ typedef struct afs_event {
     thread_t owner;
 # endif
     /* no cond member */
-#elif defined(AFS_FBSD_ENV) || defined(AFS_OBSD_ENV)
+#elif defined(AFS_OBSD_ENV)
     int cond;			/* "all this gluck should probably be replaced by CVs" */
+#elif defined(AFS_FBSD_ENV)
+    struct cv cond;
 #elif defined(AFS_LINUX24_ENV)
     wait_queue_head_t cond;
 #elif defined(AFS_NBSD_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_SGI_ENV)
