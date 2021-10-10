--- src/rx/rx.h.orig	2021-07-29 10:24:31 UTC
+++ src/rx/rx.h
@@ -416,9 +416,22 @@ struct rx_ackPacket {
     afs_uint32 previousPacket;	/* The previous packet number received (obsolete?) */
     afs_uint32 serial;		/* Serial number of the packet which prompted the acknowledge */
     u_char reason;		/* Reason for the acknowledge of ackPacket, defined below */
-    u_char nAcks;		/* Number of acknowledgements */
-    u_char acks[RX_MAXACKS];	/* Up to RX_MAXACKS packet acknowledgements, defined below */
-    /* Packets <firstPacket are implicitly acknowledged and may be discarded by the sender.  Packets >= firstPacket+nAcks are implicitly NOT acknowledged.  No packets with sequence numbers >= firstPacket should be discarded by the sender (they may thrown out at any time by the receiver) */
+    u_char nAcks;		/* Number of acknowledgements (saturates at 255) */
+    u_char acks[RX_MAXACKS];	/* Packet acknowledgements, one bit per packet.
+				 * The first (up to) RX_MAXACKS packets'
+				 * acknowledgment state is indicated by bit-0
+				 * of the corresponding byte of acks[].  The
+				 * additional bits are reserved for future use. */
+    /*
+     * DATA packets whose sequence number is less than firstPacket are
+     * implicitly acknowledged and may be discarded by the sender.
+     * DATA packets whose sequence number is greater than or equal to
+     * (firstPacket + nAcks) are implicitly NOT acknowledged.
+     * No DATA packets with sequence numbers greater than or equal to
+     * firstPacket should be discarded by the sender (they may be thrown
+     * out by the receiver and listed as NOT acknowledged in a subsequent
+     * ACK packet.)
+     */
 };
 
 #define FIRSTACKOFFSET 4
@@ -436,9 +449,9 @@ struct rx_ackPacket {
 					 * be used to compute RTT */
 #define RX_ACK_MTU             -1       /* will be rewritten to ACK_PING */
 
-/* Packet acknowledgement type */
-#define	RX_ACK_TYPE_NACK	0	/* I Don't have this packet */
-#define	RX_ACK_TYPE_ACK		1	/* I have this packet, although I may discard it later */
+/* Packet acknowledgement type (for maximum window size 255) */
+#define RX_ACK_TYPE_NACK        0x0     /* I Don't have this packet */
+#define RX_ACK_TYPE_ACK         0x1     /* I have this packet, although I may discard it later */
 
 /* The packet size transmitted for an acknowledge is adjusted to reflect the actual size of the acks array.  This macro defines the size */
 #define rx_AckDataSize(nAcks) (3 + nAcks + offsetof(struct rx_ackPacket, acks[0]))
