--- src/rx/DARWIN/rx_knet.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/DARWIN/rx_knet.c
@@ -152,7 +152,6 @@ rx_upcall(socket_t so, void *arg, __unused int waitfla
 void
 osi_StopNetIfPoller(void)
 {
-    shutdown_rx();
     soclose(rx_socket);
     if (afs_termState == AFSOP_STOP_NETIF) {
 	afs_termState = AFSOP_STOP_COMPLETE;
