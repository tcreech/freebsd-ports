--- src/afs/afs_mariner.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_mariner.c
@@ -104,7 +104,7 @@ afs_MarinerLog(char *astring, struct vcache *avc)
     dvec.iov_base = buf;
     dvec.iov_len = tp - buf;
     AFS_GUNLOCK();
-    (void)osi_NetSend(afs_server->socket, &taddr, &dvec, 1, tp - buf, 0);
+    (void)rxi_NetSend(afs_server->socket, &taddr, &dvec, 1, tp - buf, 0);
     AFS_GLOCK();
     osi_FreeSmallSpace(buf);
 }				/*afs_MarinerLog */
