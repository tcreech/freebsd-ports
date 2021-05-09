--- src/rx/rx_lwp.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_lwp.c
@@ -413,12 +413,9 @@ rxi_Recvmsg(osi_socket socket, struct msghdr *msg_p, i
     int code;
     code = recvmsg(socket, msg_p, flags);
 
-#ifdef AFS_RXERRQ_ENV
     if (code < 0) {
-	while((rxi_HandleSocketError(socket)) > 0)
-	    ;
+	rxi_HandleSocketErrors(socket);
     }
-#endif
 
     return code;
 }
@@ -451,10 +448,6 @@ rxi_Sendmsg(osi_socket socket, struct msghdr *msg_p, i
 	    }
 	    FD_SET(socket, sfds);
 	}
-#ifdef AFS_RXERRQ_ENV
-	while((rxi_HandleSocketError(socket)) > 0)
-	  ;
-#endif
 #ifdef AFS_NT40_ENV
 	if (err)
 #elif defined(AFS_LINUX22_ENV)
