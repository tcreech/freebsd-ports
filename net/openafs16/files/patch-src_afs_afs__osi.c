--- src/afs/afs_osi.c.orig	2017-02-26 23:30:15 UTC
+++ src/afs/afs_osi.c
@@ -57,7 +57,7 @@ struct lock__bsd__ afs_global_lock;
 struct lock afs_global_lock;
 afs_proc_t *afs_global_owner;
 #elif defined(AFS_FBSD_ENV)
-struct mtx afs_global_mtx;
+struct sx afs_global_sx;
 struct thread *afs_global_owner;
 #endif
 
