--- src/afs/afs_callback.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/afs_callback.c
@@ -980,7 +980,9 @@ afs_RXCallBackServer(void)
     /*
      * Donate this process to Rx.
      */
+    AFS_GUNLOCK();
     rx_ServerProc(NULL);
+    AFS_GLOCK();
     return (0);
 
 }				/*afs_RXCallBackServer */
