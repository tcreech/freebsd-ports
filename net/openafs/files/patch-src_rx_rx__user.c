--- src/rx/rx_user.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_user.c
@@ -781,21 +781,20 @@ rx_SetMaxMTU(int mtu)
 
 #ifdef AFS_RXERRQ_ENV
 int
-rxi_HandleSocketError(int socket)
+osi_HandleSocketError(int socket, void *cmsgbuf, size_t cmsgbuf_len)
 {
     struct msghdr msg;
     struct cmsghdr *cmsg;
     struct sock_extended_err *err;
     struct sockaddr_in addr;
-    char controlmsgbuf[256];
     int code;
 
     msg.msg_name = &addr;
     msg.msg_namelen = sizeof(addr);
     msg.msg_iov = NULL;
     msg.msg_iovlen = 0;
-    msg.msg_control = controlmsgbuf;
-    msg.msg_controllen = 256;
+    msg.msg_control = cmsgbuf;
+    msg.msg_controllen = cmsgbuf_len;
     msg.msg_flags = 0;
     code = recvmsg(socket, &msg, MSG_ERRQUEUE|MSG_DONTWAIT|MSG_TRUNC);
 
