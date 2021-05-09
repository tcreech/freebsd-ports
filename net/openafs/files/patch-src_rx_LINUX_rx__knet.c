--- src/rx/LINUX/rx_knet.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/LINUX/rx_knet.c
@@ -34,7 +34,6 @@
 #include <linux/errqueue.h>
 #include <linux/icmp.h>
 #endif
-
 #include "osi_compat.h"
 
 /* rxk_NewSocket
@@ -76,14 +75,10 @@ rxk_NewSocketHost(afs_uint32 ahost, short aport)
 	return NULL;
     }
 
-    kernel_setsockopt(sockp, SOL_IP, IP_MTU_DISCOVER, (char *)&pmtu,
-		      sizeof(pmtu));
+    afs_linux_sock_set_mtu_discover(sockp, pmtu);
+
 #ifdef AFS_RXERRQ_ENV
-    {
-	int recverr = 1;
-	kernel_setsockopt(sockp, SOL_IP, IP_RECVERR, (char *)&recverr,
-	                  sizeof(recverr));
-    }
+    afs_linux_sock_set_recverr(sockp);
 #endif
     return (osi_socket *)sockp;
 }
@@ -103,8 +98,8 @@ rxk_FreeSocket(struct socket *asocket)
 }
 
 #ifdef AFS_RXERRQ_ENV
-static int
-osi_HandleSocketError(osi_socket so, char *cmsgbuf, size_t cmsgbuf_len)
+int
+osi_HandleSocketError(osi_socket so, void *cmsgbuf, size_t cmsgbuf_len)
 {
     struct msghdr msg;
     struct cmsghdr *cmsg;
@@ -145,26 +140,6 @@ osi_HandleSocketError(osi_socket so, char *cmsgbuf, si
 }
 #endif
 
-static void
-do_handlesocketerror(osi_socket so)
-{
-#ifdef AFS_RXERRQ_ENV
-    char *cmsgbuf;
-    size_t cmsgbuf_len;
-
-    cmsgbuf_len = 256;
-    cmsgbuf = rxi_Alloc(cmsgbuf_len);
-    if (!cmsgbuf) {
-	return;
-    }
-
-    while (osi_HandleSocketError(so, cmsgbuf, cmsgbuf_len))
-	;
-
-    rxi_Free(cmsgbuf, cmsgbuf_len);
-#endif
-}
-
 /* osi_NetSend
  *
  * Return codes:
@@ -187,10 +162,6 @@ osi_NetSend(osi_socket sop, struct sockaddr_in *to, st
 
     code = kernel_sendmsg(sop, &msg, (struct kvec *) iovec, iovcnt, size);
 
-    if (code < 0) {
-	do_handlesocketerror(sop);
-    }
-
     return (code < 0) ? code : 0;
 }
 
@@ -255,7 +226,7 @@ osi_NetReceive(osi_socket so, struct sockaddr_in *from
 	rxk_lastSocketError = code;
 	rxk_nSocketErrors++;
 
-	do_handlesocketerror(so);
+	rxi_HandleSocketErrors(so);
     } else {
 	*lengthp = code;
 	code = 0;
