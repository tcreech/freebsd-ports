--- src/afs/afs_callback.c.orig	2022-12-15 20:10:23 UTC
+++ src/afs/afs_callback.c
@@ -983,7 +983,9 @@ afs_RXCallBackServer(void)
     /*
      * Donate this process to Rx.
      */
+    AFS_GUNLOCK();
     rx_ServerProc(NULL);
+    AFS_GLOCK();
     return (0);
 
 }				/*afs_RXCallBackServer */
