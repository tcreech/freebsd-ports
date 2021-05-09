--- src/rx/rx.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx.c
@@ -135,7 +135,7 @@ static struct rx_packet
 			       struct rx_call **newcallp);
 static struct rx_packet
 	*rxi_ReceiveAckPacket(struct rx_call *call, struct rx_packet *np,
-			      int istack);
+			      int istack, int *a_invalid);
 static struct rx_packet
 	*rxi_ReceiveResponsePacket(struct rx_connection *conn,
 				   struct rx_packet *np, int istack);
@@ -2044,7 +2044,7 @@ rx_GetCall(int tno, struct rx_service *cur_service, os
 		}
 		MUTEX_ENTER(&rx_pthread_mutex);
 		if (tno == rxi_fcfs_thread_num
-			|| opr_queue_IsEnd(&rx_incomingCallQueue, cursor)) {
+			|| opr_queue_IsLast(&rx_incomingCallQueue, cursor)) {
 		    MUTEX_EXIT(&rx_pthread_mutex);
 		    /* If we're the fcfs thread , then  we'll just use
 		     * this call. If we haven't been able to find an optimal
@@ -2217,7 +2217,7 @@ rx_GetCall(int tno, struct rx_service *cur_service, os
 	    if (QuotaOK(service)) {
 		MUTEX_ENTER(&rx_pthread_mutex);
 		if (tno == rxi_fcfs_thread_num
-			|| opr_queue_IsEnd(&rx_incomingCallQueue, cursor)) {
+			|| opr_queue_IsLast(&rx_incomingCallQueue, cursor)) {
 		    MUTEX_EXIT(&rx_pthread_mutex);
 		    /* If we're the fcfs thread, then  we'll just use
 		     * this call. If we haven't been able to find an optimal
@@ -2833,7 +2833,7 @@ rxi_Alloc(size_t size)
     }
 
 p = (char *)
-#if defined(KERNEL) && !defined(UKERNEL) && defined(AFS_FBSD80_ENV)
+#if defined(KERNEL) && !defined(UKERNEL) && defined(AFS_FBSD_ENV)
   afs_osi_Alloc_NoSleep(size);
 #else
   osi_Alloc(size);
@@ -3266,6 +3266,36 @@ rxi_ReceiveServerCall(osi_socket socket, struct rx_pac
     call = conn->call[channel];
 
     if (!call) {
+	if (np->header.type != RX_PACKET_TYPE_DATA) {
+	    /*
+	     * Clients must send DATA packets at some point to create a new
+	     * call. If the first packet we saw for this call channel is
+	     * something else, then either the DATA packets got lost/delayed,
+	     * or we were restarted and this is an existing call from before we
+	     * were restarted. In the latter case, some clients get confused if
+	     * we respond to such requests, so just drop the packet to make
+	     * things easier for them.
+	     */
+	    MUTEX_EXIT(&conn->conn_call_lock);
+	    if (rx_stats_active)
+		rx_atomic_inc(&rx_stats.spuriousPacketsRead);
+	    return NULL;
+	}
+
+	if (np->header.seq > rx_maxReceiveWindow) {
+	    /*
+	     * This is a DATA packet for further along in the call than is
+	     * possible for a new call. This is probably from an existing call
+	     * that was in the middle of running when we were restarted; ignore
+	     * it to avoid confusing clients. (See above comment about non-DATA
+	     * packets.)
+	     */
+	    MUTEX_EXIT(&conn->conn_call_lock);
+	    if (rx_stats_active)
+		rx_atomic_inc(&rx_stats.spuriousPacketsRead);
+	    return NULL;
+	}
+
 	if (rxi_AbortIfServerBusy(socket, conn, np)) {
 	    MUTEX_EXIT(&conn->conn_call_lock);
 	    return NULL;
@@ -3377,6 +3407,7 @@ rxi_ReceivePacket(struct rx_packet *np, osi_socket soc
     struct rx_connection *conn;
     int type;
     int unknownService = 0;
+    int invalid = 0;
 #ifdef RXDEBUG
     char *packetType;
 #endif
@@ -3439,7 +3470,7 @@ rxi_ReceivePacket(struct rx_packet *np, osi_socket soc
 	memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));
 #ifdef STRUCT_SOCKADDR_HAS_SA_LEN
 	addr.sin_len = sizeof(addr);
-#endif /* AFS_OSF_ENV */
+#endif
 	drop = (*rx_justReceived) (np, &addr);
 	/* drop packet if return value is non-zero */
 	if (drop)
@@ -3567,7 +3598,7 @@ rxi_ReceivePacket(struct rx_packet *np, osi_socket soc
 		(void)rxi_SendAck(call, 0, np->header.serial,
 				  RX_ACK_PING_RESPONSE, 1);
 	}
-	np = rxi_ReceiveAckPacket(call, np, 1);
+	np = rxi_ReceiveAckPacket(call, np, 1, &invalid);
 	break;
     case RX_PACKET_TYPE_ABORT: {
 	/* An abort packet: reset the call, passing the error up to the user. */
@@ -3600,11 +3631,16 @@ rxi_ReceivePacket(struct rx_packet *np, osi_socket soc
 	np = rxi_SendCallAbort(call, np, 1, 0);
 	break;
     };
-    /* Note when this last legitimate packet was received, for keep-alive
-     * processing.  Note, we delay getting the time until now in the hope that
-     * the packet will be delivered to the user before any get time is required
-     * (if not, then the time won't actually be re-evaluated here). */
-    call->lastReceiveTime = clock_Sec();
+    if (invalid) {
+	if (rx_stats_active)
+	    rx_atomic_inc(&rx_stats.spuriousPacketsRead);
+    } else {
+	/*
+	 * Note when this last legitimate packet was received, for keep-alive
+	 * processing.
+	 */
+	call->lastReceiveTime = clock_Sec();
+    }
     MUTEX_EXIT(&call->lock);
     putConnection(conn);
     return np;
@@ -4208,11 +4244,46 @@ rx_ack_reason(int reason)
 }
 #endif
 
+static_inline int
+ack_is_valid(struct rx_call *call, afs_uint32 first, afs_uint32 prev)
+{
+    if (first < call->tfirst) {
+	/*
+	 * The peer indicated that the window went backwards. That's not
+	 * allowed; the window can only move forwards.
+	 */
+	return 0;
+    }
 
+    if (first == call->tfirst && prev < call->tprev) {
+	/*
+	 * The peer said the last DATA packet it received was seq X, but it
+	 * already told us before that it had received data after X. This is
+	 * probably just an out-of-order ACK, and so we can ignore it.
+	 */
+	if (prev >= call->tfirst + call->twind) {
+	    /*
+	     * Some peers (OpenAFS libafs before 1.6.23) mistakenly set the
+	     * previousPacket field to a serial number, not a sequence number.
+	     * The sequence number the peer told us about is further than our
+	     * transmit window, so it cannot possibly be correct; it's probably
+	     * actually a serial number. Don't ignore packets based on this;
+	     * the previousPacket information is not accurate.
+	     */
+	    return 1;
+	}
+
+	return 0;
+    }
+
+    /* Otherwise, the ack looks valid. */
+    return 1;
+}
+
 /* The real smarts of the whole thing.  */
 static struct rx_packet *
 rxi_ReceiveAckPacket(struct rx_call *call, struct rx_packet *np,
-		     int istack)
+		     int istack, int *a_invalid)
 {
     struct rx_ackPacket *ap;
     int nAcks;
@@ -4233,6 +4304,8 @@ rxi_ReceiveAckPacket(struct rx_call *call, struct rx_p
     int pktsize = 0;            /* Set if we need to update the peer mtu */
     int conn_data_locked = 0;
 
+    *a_invalid = 1;
+
     if (rx_stats_active)
         rx_atomic_inc(&rx_stats.ackPacketsRead);
     ap = (struct rx_ackPacket *)rx_DataOf(np);
@@ -4246,19 +4319,14 @@ rxi_ReceiveAckPacket(struct rx_call *call, struct rx_p
     prev = ntohl(ap->previousPacket);
     serial = ntohl(ap->serial);
 
-    /*
-     * Ignore ack packets received out of order while protecting
-     * against peers that set the previousPacket field to a packet
-     * serial number instead of a sequence number.
-     */
-    if (first < call->tfirst ||
-        (first == call->tfirst && prev < call->tprev && prev < call->tfirst
-	 + call->twind)) {
+    if (!ack_is_valid(call, first, prev)) {
 	return np;
     }
 
     call->tprev = prev;
 
+    *a_invalid = 0;
+
     if (np->header.flags & RX_SLOW_START_OK) {
 	call->flags |= RX_CALL_SLOW_START_OK;
     }
@@ -4389,7 +4457,7 @@ rxi_ReceiveAckPacket(struct rx_call *call, struct rx_p
 
 #ifdef RX_ENABLE_LOCKS
 	/* XXX Hack. Because we have to release the global call lock when sending
-	 * packets (osi_NetSend) we drop all acks while we're traversing the tq
+	 * packets (rxi_NetSend) we drop all acks while we're traversing the tq
 	 * in rxi_Start sending packets out because packets may move to the
 	 * freePacketQueue as result of being here! So we drop these packets until
 	 * we're safely out of the traversing. Really ugly!
@@ -6452,7 +6520,7 @@ rxi_NatKeepAliveEvent(struct rxevent *event, void *arg
     tmpiov[0].iov_base = tbuffer;
     tmpiov[0].iov_len = 1 + sizeof(struct rx_header);
 
-    osi_NetSend(socket, &taddr, tmpiov, 1, 1 + sizeof(struct rx_header), 1);
+    rxi_NetSend(socket, &taddr, tmpiov, 1, 1 + sizeof(struct rx_header), 1);
 
     MUTEX_ENTER(&conn->conn_data_lock);
     /* We ran, so the handle is no longer needed to try to cancel ourselves. */
@@ -9337,3 +9405,83 @@ int rx_DumpCalls(FILE *outputFile, char *cookie)
     return 0;
 }
 #endif
+
+#ifdef AFS_RXERRQ_ENV
+void
+rxi_HandleSocketErrors(osi_socket sock)
+{
+    size_t cmsgbuf_len = 256;
+    void *cmsgbuf;
+# ifndef KERNEL
+    int errno_save = errno;
+# endif
+
+    cmsgbuf = rxi_Alloc(cmsgbuf_len);
+    if (cmsgbuf == NULL) {
+	goto done;
+    }
+
+    while (osi_HandleSocketError(sock, cmsgbuf, cmsgbuf_len))
+	;
+
+    rxi_Free(cmsgbuf, cmsgbuf_len);
+
+ done:
+# ifndef KERNEL
+    errno = errno_save;
+# endif
+    return;
+}
+
+static int
+NetSend_retry(osi_socket sock, void *addr, struct iovec *dvec, int nvecs,
+	      int length, int istack)
+{
+    int code;
+    int safety;
+    /*
+     * If an ICMP error comes in for any peer, sendmsg() can return -1 with an
+     * errno of EHOSTUNREACH, ENETUNREACH, etc. There may be no problem with
+     * sending this packet (an error is returned just to indicate we need to
+     * read in pending errors), but the packet wasn't actually sent.
+     *
+     * It's difficult to determine in general whether sendmsg() is returning an
+     * error due to a received ICMP error, or we're getting an actual error for
+     * this specific sendmsg() call, since there may be other threads running
+     * sendmsg/recvmsg/rxi_HandleSocketErrors at the same time. So, just retry
+     * the sendmsg a few times; make sure not to retry forever, in case we are
+     * getting an actual error from this sendmsg() call.
+     *
+     * Also note that if we accidentally drop a packet here that we didn't need
+     * to, it's not the end of the world. Packets get dropped, and we should be
+     * able to recover.
+     */
+    for (safety = 0; safety < RXI_SENDMSG_RETRY; safety++) {
+	code = osi_NetSend(sock, addr, dvec, nvecs, length, istack);
+	if (code == 0) {
+	    return 0;
+	}
+	rxi_HandleSocketErrors(sock);
+    }
+    return code;
+
+}
+#endif
+
+int
+rxi_NetSend(osi_socket socket, void *addr, struct iovec *dvec,
+	    int nvecs, int length, int istack)
+{
+    if (rxi_IsRunning()) {
+#ifdef AFS_RXERRQ_ENV
+	return NetSend_retry(socket, addr, dvec, nvecs, length, istack);
+#else
+	return osi_NetSend(socket, addr, dvec, nvecs, length, istack);
+#endif
+    }
+#ifdef AFS_NT40_ENV
+    return WSAESHUTDOWN;
+#else
+    return ESHUTDOWN;
+#endif
+}
