--- src/rx/rx_packet.h.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_packet.h
@@ -72,7 +72,6 @@
  * the MAX packet size will be the maximum receive size, but the maximum send
  * size will be larger than that. */
 
-#ifdef notdef
 /*  some sample MTUs
            4352   what FDDI(RFC1188) uses... Larger?
            4096   VJ's recommendation for FDDI
@@ -85,23 +84,6 @@
 	   9180   Classical IP over ATM (RFC2225)
 */
 
-/* * * * these are the old defines
-*/
-#define	RX_MAX_PACKET_SIZE	(RX_MAX_DL_MTU -RX_IPUDP_SIZE)
-
-#define	RX_MAX_PACKET_DATA_SIZE	(RX_MAX_PACKET_SIZE-RX_HEADER_SIZE)
-#ifdef AFS_HPUX_ENV
-/* HPUX by default uses an 802.3 size, and it's not evident from SIOCGIFCONF */
-#define	RX_LOCAL_PACKET_SIZE	(1492 - RX_IPUDP_SIZE)
-#define	RX_REMOTE_PACKET_SIZE	(1492 - RX_IPUDP_SIZE)
-#else
-#define	RX_LOCAL_PACKET_SIZE	RX_MAX_PACKET_SIZE	/* For hosts on same net */
-#define	RX_REMOTE_PACKET_SIZE	RX_MAX_PACKET_SIZE	/* see note above */
-#endif
-#endif /* notdef */
-
-/* These are the new, streamlined ones.
- */
 #define	RX_HEADER_SIZE		sizeof (struct rx_header)
 
 /* The minimum MTU for an IP network is 576 bytes including headers */
