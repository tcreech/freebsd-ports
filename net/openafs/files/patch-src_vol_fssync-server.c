--- src/vol/fssync-server.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/fssync-server.c
@@ -79,9 +79,6 @@
 #endif /* USE_UNIX_SOCKETS */
 
 #ifdef FSSYNC_BUILD_SERVER
-
-int (*V_BreakVolumeCallbacks) (VolumeId volume);
-
 #define MAXHANDLERS	4	/* Up to 4 clients; must be at least 2, so that
 				 * move = dump+restore can run on single server */
 #define MAXOFFLINEVOLUMES 128	/* This needs to be as big as the maximum
@@ -261,7 +258,7 @@ FSYNC_sync(void * args)
     /* set our 'thread-id' so that the host hold table works */
     tid = rx_SetThreadNum();
     Log("Set thread id %d for FSYNC_sync\n", tid);
-    afs_pthread_setname_self("FSYNC_sync");
+    opr_threadname_set("FSYNC_sync");
 #endif /* AFS_PTHREAD_ENV */
 
     VOL_LOCK;
