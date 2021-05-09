--- src/rx/rx.h.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx.h
@@ -365,7 +365,8 @@ struct rx_service {
 #define RX_CONN_BUSY               32	/* connection is busy; don't delete */
 #define RX_CONN_ATTACHWAIT	   64	/* attach waiting for peer->lastReach */
 #define RX_CONN_MAKECALL_ACTIVE   128   /* a thread is actively in rx_NewCall */
-#define RX_CONN_NAT_PING          256   /* nat ping requested */
+#define RX_CONN_NAT_PING          256   /* NAT ping requested but deferred during attachWait */
+#define RX_CONN_CACHED		  512   /* connection is managed by rxi_connectionCache */
 
 /* Type of connection, client or server */
 #define	RX_CLIENT_CONNECTION	0
