--- src/butc/lwps.c.orig	2021-01-14 21:08:41 UTC
+++ src/butc/lwps.c
@@ -51,7 +51,7 @@ extern int forcemultiple;
 #endif
 
 /* XBSA Global Parameters */
-afs_int32 xbsaType;
+extern afs_int32 xbsaType;
 #ifdef xbsa
 struct butx_transactionInfo butxInfo;
 #endif
@@ -1636,7 +1636,7 @@ Restorer(void *param) {
     time_t startTime, endTime;
     afs_int32 goodrestore = 0;
 
-    afs_pthread_setname_self("restorer");
+    opr_threadname_set("restorer");
     taskId = newNode->taskID;
     setStatus(taskId, DRIVE_WAIT);
     EnterDeviceQueue(deviceLatch);
@@ -2102,7 +2102,7 @@ Labeller(void *param)
     afs_uint32 taskId;
     afs_int32 code = 0;
 
-    afs_pthread_setname_self("labeller");
+    opr_threadname_set("labeller");
     taskId = labelIfPtr->taskId;
     setStatus(taskId, DRIVE_WAIT);
     EnterDeviceQueue(deviceLatch);
