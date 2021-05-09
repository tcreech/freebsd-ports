--- src/rx/rx_internal.h.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_internal.h
@@ -17,6 +17,9 @@
 extern rx_atomic_t rx_nWaiting;
 extern rx_atomic_t rx_nWaited;
 
+/* How many times to retry sendmsg()-equivalent calls for AFS_RXERRQ_ENV. */
+#define RXI_SENDMSG_RETRY 8
+
 /* Prototypes for internal functions */
 
 /* rx.c */
@@ -28,6 +31,11 @@ extern void rxi_SetPeerMtu(struct rx_peer *peer, afs_u
 #ifdef AFS_RXERRQ_ENV
 extern void rxi_ProcessNetError(struct sock_extended_err *err,
                                 afs_uint32 addr, afs_uint16 port);
+extern int osi_HandleSocketError(osi_socket sock, void *cmsgbuf,
+				 size_t cmsgbuf_len);
+extern void rxi_HandleSocketErrors(osi_socket sock);
+#else
+# define rxi_HandleSocketErrors(sock) do { } while (0)
 #endif
 extern struct rx_peer *rxi_FindPeer(afs_uint32 host, u_short port,
 				    int create);
