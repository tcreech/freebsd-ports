--- src/afs/afs_osi.c.orig	2016-12-08 04:01:51 UTC
+++ src/afs/afs_osi.c
@@ -66,7 +66,7 @@ struct lock afs_global_lock;
 afs_proc_t *afs_global_owner;
 # endif
 #elif defined(AFS_FBSD_ENV)
-struct mtx afs_global_mtx;
+struct sx afs_global_sx;
 struct thread *afs_global_owner;
 #endif
 
