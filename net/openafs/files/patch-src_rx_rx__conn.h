--- src/rx/rx_conn.h.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_conn.h
@@ -54,10 +54,11 @@ struct rx_connection {
     struct rx_service *service;	/* used by servers only */
     u_short serviceId;		/* To stamp on requests (clients only) */
     afs_int32 refCount;	        /* Reference count (rx_refcnt_mutex) */
-    u_char flags;		/* Defined below - (conn_data_lock) */
+    u_char spare;		/* was flags - placeholder for alignment */
     u_char type;		/* Type of connection, defined below */
     u_char secondsUntilPing;	/* how often to ping for each active call */
     u_char securityIndex;	/* corresponds to the security class of the */
+    afs_uint32 flags;		/* Defined in rx.h RX_CONN_* */
     /* securityObject for this conn */
     struct rx_securityClass *securityObject;	/* Security object for this connection */
     void *securityData;		/* Private data for this conn's security class */
