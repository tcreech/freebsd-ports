--- src/rx/rx.c.orig	2021-07-29 10:24:31 UTC
+++ src/rx/rx.c
@@ -4374,7 +4374,8 @@ rxi_ReceiveAckPacket(struct rx_call *call, struct rx_p
 	    int offset;
 
 	    for (offset = 0; offset < nAcks && len < sizeof(msg); offset++)
-		msg[len++] = (ap->acks[offset] == RX_ACK_TYPE_NACK ? '-' : '*');
+		msg[len++] = ((ap->acks[offset] & RX_ACK_TYPE_ACK) != 0 ? '*'
+									: '-');
 	}
 	msg[len++]='\n';
 	msg[len] = '\0';
@@ -4390,7 +4391,7 @@ rxi_ReceiveAckPacket(struct rx_call *call, struct rx_p
 	if (nAcks) {
 	    int offset;
 	    for (offset = 0; offset < nAcks; offset++)
-		putc(ap->acks[offset] == RX_ACK_TYPE_NACK ? '-' : '*',
+		putc((ap->acks[offset] & RX_ACK_TYPE_ACK) != 0 ? '*' : '-',
 		     rx_Log);
 	}
 	putc('\n', rx_Log);
@@ -4506,7 +4507,7 @@ rxi_ReceiveAckPacket(struct rx_call *call, struct rx_p
 	 * be downgraded when the server has discarded a packet it
 	 * soacked previously, or when an ack packet is received
 	 * out of sequence. */
-	if (ap->acks[tp->header.seq - first] == RX_ACK_TYPE_ACK) {
+	if ((ap->acks[tp->header.seq - first] & RX_ACK_TYPE_ACK) != 0) {
 	    if (!(tp->flags & RX_PKTFLAG_ACKED)) {
 		newAckCount++;
 		tp->flags |= RX_PKTFLAG_ACKED;
@@ -5754,7 +5755,8 @@ rxi_SendAck(struct rx_call *call,
 	    int offset;
 
 	    for (offset = 0; offset < ap->nAcks && len < sizeof(msg); offset++)
-		msg[len++] = (ap->acks[offset] == RX_ACK_TYPE_NACK ? '-' : '*');
+		msg[len++] = ((ap->acks[offset] & RX_ACK_TYPE_ACK) != 0 ? '*'
+									: '-');
 	}
 	msg[len++]='\n';
 	msg[len] = '\0';
@@ -5767,7 +5769,7 @@ rxi_SendAck(struct rx_call *call,
 		(unsigned int)p->header.seq, ntohl(ap->firstPacket));
 	if (ap->nAcks) {
 	    for (offset = 0; offset < ap->nAcks; offset++)
-		putc(ap->acks[offset] == RX_ACK_TYPE_NACK ? '-' : '*',
+		putc((ap->acks[offset] & RX_ACK_TYPE_ACK) != 0 ? '*' : '-',
 		     rx_Log);
 	}
 	putc('\n', rx_Log);
