--- src/butc/tcudbprocs.c.orig	2021-01-14 21:08:41 UTC
+++ src/butc/tcudbprocs.c
@@ -641,7 +641,7 @@ saveDbToTape(void *param)
     extern struct deviceSyncNode *deviceLatch;
     extern struct tapeConfig globalTapeConfig;
 
-    afs_pthread_setname_self("Db save");
+    opr_threadname_set("Db save");
     expires = (saveDbIfPtr->archiveTime ? NEVERDATE : 0);
     taskId = saveDbIfPtr->taskId;
     dumpEntry.id = 0;
@@ -1022,7 +1022,7 @@ restoreDbFromTape(void *param)
     extern struct tapeConfig globalTapeConfig;
     extern struct deviceSyncNode *deviceLatch;
 
-    afs_pthread_setname_self("Db restore");
+    opr_threadname_set("Db restore");
     setStatus(taskId, DRIVE_WAIT);
     EnterDeviceQueue(deviceLatch);	/* lock tape device */
     clearStatus(taskId, DRIVE_WAIT);
@@ -1120,7 +1120,7 @@ KeepAlive(void *unused)
 
     extern struct udbHandleS udbHandle;
 
-    afs_pthread_setname_self("Keep-alive");
+    opr_threadname_set("Keep-alive");
     while (1) {
 #ifdef AFS_PTHREAD_ENV
 	sleep(5);
