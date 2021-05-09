--- src/xstat/xstat_cm.c.orig	2021-01-14 21:08:41 UTC
+++ src/xstat/xstat_cm.c
@@ -32,9 +32,8 @@
 int xstat_cm_numServers;	/*Num connected servers */
 struct xstat_cm_ConnectionInfo
  *xstat_cm_ConnInfo;		/*Ptr to connection array */
-int numCollections;		/*Number of data collections */
 struct xstat_cm_ProbeResults xstat_cm_Results;	/*Latest probe results */
-char terminationEvent;		/*One-shot termination event */
+char cm_terminationEvent;		/*One-shot termination event */
 
 afs_int32 xstat_cmData[AFSCB_MAX_XSTAT_LONGS];	/*Buffer for collected data */
 
@@ -315,8 +314,8 @@ xstat_cm_LWP(void *unused)
 	     */
 	    if (xstat_cm_debug)
 		printf("[%s] Signalling main process at %" AFS_PTR_FMT "\n", rn,
-		       &terminationEvent);
-	    oneShotCode = LWP_SignalProcess(&terminationEvent);
+		       &cm_terminationEvent);
+	    oneShotCode = LWP_SignalProcess(&cm_terminationEvent);
 	    if (oneShotCode)
 		fprintf(stderr, "[%s] Error %d from LWP_SignalProcess()", rn,
 			oneShotCode);
