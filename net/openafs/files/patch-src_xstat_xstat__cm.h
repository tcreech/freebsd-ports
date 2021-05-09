--- src/xstat/xstat_cm.h.orig	2021-01-14 21:08:41 UTC
+++ src/xstat/xstat_cm.h
@@ -86,10 +86,9 @@ struct xstat_cm_ProbeResults {
 extern int xstat_cm_numServers;	/*# connected servers */
 extern struct xstat_cm_ConnectionInfo
  *xstat_cm_ConnInfo;		/*Ptr to connections */
-extern int numCollections;	/*Num data collections */
 extern struct xstat_cm_ProbeResults
   xstat_cm_Results;		/*Latest probe results */
-extern char terminationEvent;	/*One-shot termination event */
+extern char cm_terminationEvent;	/*One-shot termination event */
 
 /*
  * ------------------------ Exported functions ------------------------
