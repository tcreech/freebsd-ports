--- src/butc/recoverDb.c.orig	2021-01-14 21:08:41 UTC
+++ src/butc/recoverDb.c
@@ -12,6 +12,7 @@
 
 #include <roken.h>
 
+#include <afs/opr.h>
 #include <rx/xdr.h>
 #include <rx/rx.h>
 #include <lwp.h>
@@ -660,15 +661,6 @@ getScanTape(afs_int32 taskId, struct butm_tapeInfo *ta
 	/* If no name, accept any tape */
 	if (strcmp(tname, "") == 0) {
 	    break;		/* Start scan on any tape */
-#ifdef notdef
-	    if (curseq == 1)
-		break;		/* The first tape */
-	    else {
-		TLog(taskId, "Expected first tape of dump, label seen %s\n",
-		     gotname);
-		goto newtape;
-	    }
-#endif
 	}
 
 	if (strcmp(tname, tapeLabelPtr->AFSName)
@@ -706,7 +698,7 @@ ScanDumps(void *param)
     afs_uint32 taskId;
     afs_int32 code = 0;
 
-    afs_pthread_setname_self("scandump");
+    opr_threadname_set("scandump");
     taskId = ptr->taskId;
     setStatus(taskId, DRIVE_WAIT);
     EnterDeviceQueue(deviceLatch);
