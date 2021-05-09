--- src/volser/volmain.c.orig	2021-01-14 21:08:41 UTC
+++ src/volser/volmain.c
@@ -134,7 +134,7 @@ BKGLoop(void *unused)
     struct timeval tv;
     int loop = 0;
 
-    afs_pthread_setname_self("vol bkg");
+    opr_threadname_set("vol bkg");
     while (1) {
 	tv.tv_sec = GCWAKEUP;
 	tv.tv_usec = 0;
