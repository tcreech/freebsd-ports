--- src/xstat/xstat_fs.h.orig	2021-01-14 21:08:41 UTC
+++ src/xstat/xstat_fs.h
@@ -87,10 +87,9 @@ struct xstat_fs_ProbeResults {
 extern int xstat_fs_numServers;	/*# connected servers */
 extern struct xstat_fs_ConnectionInfo
  *xstat_fs_ConnInfo;		/*Ptr to connections */
-extern int numCollections;	/*Num data collections */
 extern struct xstat_fs_ProbeResults
   xstat_fs_Results;		/*Latest probe results */
-extern char terminationEvent;	/*One-shot termination event */
+extern char fs_terminationEvent;	/*One-shot termination event */
 
 /*
  * ------------------------ Exported functions ------------------------
