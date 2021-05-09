--- src/xstat/xstat_fs.c.orig	2021-01-14 21:08:41 UTC
+++ src/xstat/xstat_fs.c
@@ -33,9 +33,8 @@
 int xstat_fs_numServers;	/*Num connected servers */
 struct xstat_fs_ConnectionInfo
  *xstat_fs_ConnInfo;		/*Ptr to connection array */
-int numCollections;		/*Number of data collections */
 struct xstat_fs_ProbeResults xstat_fs_Results;	/*Latest probe results */
-char terminationEvent;		/*One-shot termination event */
+char fs_terminationEvent;		/*One-shot termination event */
 
 afs_int32 xstat_fsData[AFS_MAX_XSTAT_LONGS];	/*Buffer for collected data */
 
@@ -334,8 +333,8 @@ xstat_fs_LWP(void *unused)
 	     */
 	    if (xstat_fs_debug)
 		printf("[%s] Signalling main process at %" AFS_PTR_FMT "\n", rn,
-		       &terminationEvent);
-	    oneShotCode = LWP_SignalProcess(&terminationEvent);
+		       &fs_terminationEvent);
+	    oneShotCode = LWP_SignalProcess(&fs_terminationEvent);
 	    if (oneShotCode)
 		fprintf(stderr, "[%s] Error %d from LWP_SignalProcess()", rn,
 			oneShotCode);
