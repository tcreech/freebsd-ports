--- src/rx/rx_prototypes.h.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_prototypes.h
@@ -412,6 +412,8 @@ extern struct rx_packet *rxi_SplitJumboPacket(struct r
 extern int osi_NetSend(osi_socket socket, void *addr, struct iovec *dvec,
 		       int nvecs, int length, int istack);
 #endif
+extern int rxi_NetSend(osi_socket socket, void *addr, struct iovec *dvec,
+		       int nvecs, int length, int istack);
 extern struct rx_packet *rxi_ReceiveDebugPacket(struct rx_packet *ap,
 						osi_socket asocket,
 						afs_uint32 ahost, short aport,
@@ -500,7 +502,6 @@ extern afs_kmutex_t rx_if_mutex;
 #endif
 extern osi_socket rxi_GetUDPSocket(u_short port);
 extern void rxi_InitPeerParams(struct rx_peer *pp);
-extern int rxi_HandleSocketError(int socket);
 
 #if defined(AFS_AIX32_ENV) && !defined(KERNEL)
 #ifndef osi_Alloc
