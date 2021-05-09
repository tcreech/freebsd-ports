--- src/rx/rx_pthread.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_pthread.c
@@ -393,12 +393,9 @@ rxi_Recvmsg(osi_socket socket, struct msghdr *msg_p, i
     int ret;
     ret = recvmsg(socket, msg_p, flags);
 
-#ifdef AFS_RXERRQ_ENV
     if (ret < 0) {
-	while (rxi_HandleSocketError(socket) > 0)
-	    ;
+	rxi_HandleSocketErrors(socket);
     }
-#endif
 
     return ret;
 }
@@ -409,37 +406,34 @@ rxi_Recvmsg(osi_socket socket, struct msghdr *msg_p, i
 int
 rxi_Sendmsg(osi_socket socket, struct msghdr *msg_p, int flags)
 {
-    int ret;
-    ret = sendmsg(socket, msg_p, flags);
-
-#ifdef AFS_RXERRQ_ENV
-    if (ret < 0) {
-	while (rxi_HandleSocketError(socket) > 0)
-	    ;
-	return ret;
+    int err;
+    if (sendmsg(socket, msg_p, flags) >= 0) {
+	return 0;
     }
+
+#ifdef AFS_NT40_ENV
+    err = WSAGetLastError();
 #else
+    err = errno;
+#endif
+
+#ifndef AFS_RXERRQ_ENV
 # ifdef AFS_LINUX22_ENV
     /* linux unfortunately returns ECONNREFUSED if the target port
      * is no longer in use */
     /* and EAGAIN if a UDP checksum is incorrect */
-    if (ret == -1 && errno != ECONNREFUSED && errno != EAGAIN) {
-# else
-    if (ret == -1) {
-# endif
-	dpf(("rxi_sendmsg failed, error %d\n", errno));
-	fflush(stdout);
-# ifndef AFS_NT40_ENV
-        if (errno > 0)
-          return -errno;
-# else
-            if (WSAGetLastError() > 0)
-              return -WSAGetLastError();
-# endif
-	return -1;
+    if (err == ECONNREFUSED || err == EAGAIN) {
+	return 0;
     }
+# endif
+    dpf(("rxi_sendmsg failed, error %d\n", errno));
+    fflush(stdout);
 #endif /* !AFS_RXERRQ_ENV */
-    return 0;
+
+    if (err > 0) {
+	return -err;
+    }
+    return -1;
 }
 
 struct rx_ts_info_t * rx_ts_info_init(void) {
