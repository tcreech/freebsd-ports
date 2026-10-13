--- src/afs/afs_callback.c.orig	2024-10-03 22:32:45 UTC
+++ src/afs/afs_callback.c
@@ -985,7 +985,9 @@ afs_RXCallBackServer(void)
     /*
      * Donate this process to Rx.
      */
+    AFS_GUNLOCK();
     rx_ServerProc(NULL);
+    AFS_GLOCK();
     return (0);
 
 }				/*afs_RXCallBackServer */
