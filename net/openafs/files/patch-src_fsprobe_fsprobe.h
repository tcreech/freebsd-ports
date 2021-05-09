--- src/fsprobe/fsprobe.h.orig	2021-01-14 21:08:41 UTC
+++ src/fsprobe/fsprobe.h
@@ -103,7 +103,6 @@ struct fsprobe_ProbeResults {
 
 extern int fsprobe_numServers;	/*# servers connected */
 extern struct fsprobe_ConnectionInfo *fsprobe_ConnInfo;	/*Ptr to connections */
-extern int numCollections;	/*Num data collections */
 extern struct fsprobe_ProbeResults fsprobe_Results;	/*Latest probe results */
 
 extern int fsprobe_Init(int, struct sockaddr_in *, int, int (*)(void), int );
