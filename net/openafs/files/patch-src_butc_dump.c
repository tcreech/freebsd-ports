--- src/butc/dump.c.orig	2021-01-14 21:08:41 UTC
+++ src/butc/dump.c
@@ -12,6 +12,7 @@
 
 #include <roken.h>
 
+#include <afs/opr.h>
 #include <rx/xdr.h>
 #include <rx/rx.h>
 #include <lwp.h>
@@ -42,8 +43,7 @@ extern int isafile;
 extern int forcemultiple;
 
 extern struct ubik_client *cstruct;
-dlqlinkT savedEntries;
-dlqlinkT entries_to_flush;
+extern dlqlinkT savedEntries;
 
 extern afs_int32 groupId;
 extern afs_int32 BufferSize;
@@ -61,8 +61,8 @@ extern char *xbsalGName;
 extern char *globalButcLog;
 #endif /*xbsa */
 
-afs_int32 dataSize;		/* Size of data to read on each rx_Read() call */
-afs_int32 tapeblocks;		/* Number of 16K tape datablocks in buffer (!CONF_XBSA) */
+extern afs_int32 dataSize;		/* Size of data to read on each rx_Read() call */
+extern afs_int32 tapeblocks;		/* Number of 16K tape datablocks in buffer (!CONF_XBSA) */
 
 /* TBD
  *
@@ -1116,7 +1116,7 @@ Dumper(void *param)
     extern struct deviceSyncNode *deviceLatch;
     extern struct tapeConfig globalTapeConfig;
 
-    afs_pthread_setname_self("dumper");
+    opr_threadname_set("dumper");
     taskId = nodePtr->taskID;	/* Get task Id */
     setStatus(taskId, DRIVE_WAIT);
     EnterDeviceQueue(deviceLatch);
@@ -2033,7 +2033,7 @@ DeleteDump(void *param)
     dumpid = ptr->dumpID;
     taskId = ptr->taskId;	/* Get task Id */
 
-    afs_pthread_setname_self("deletedump");
+    opr_threadname_set("deletedump");
     setStatus(taskId, DRIVE_WAIT);
     EnterDeviceQueue(deviceLatch);
     clearStatus(taskId, DRIVE_WAIT);
