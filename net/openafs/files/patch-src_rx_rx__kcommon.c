--- src/rx/rx_kcommon.c.orig	2016-03-16 14:31:22 UTC
+++ src/rx/rx_kcommon.c
@@ -23,6 +23,11 @@
 #endif
 #include "afsint.h"
 
+#if defined(AFS_FBSD80_ENV)
+#include <sys/param.h>
+#include <sys/jail.h>
+#endif
+
 #ifndef RXK_LISTENER_ENV
 int (*rxk_PacketArrivalProc) (struct rx_packet * ahandle, struct sockaddr_in * afrom, struct socket *arock, afs_int32 asize);	/* set to packet allocation procedure */
 int (*rxk_GetPacketProc) (struct rx_packet **ahandle, int asize);
@@ -364,6 +369,10 @@ rxi_InitPeerParams(struct rx_peer *pp)
 {
     u_short rxmtu;
 
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_SET(TD_TO_VNET(curthread));
+#endif
+
 #ifdef	ADAPT_MTU
 # ifndef AFS_SUN5_ENV
 #  ifdef AFS_USERSPACE_IP_ADDR
@@ -456,6 +465,11 @@ rxi_InitPeerParams(struct rx_peer *pp)
     pp->cwind = 1;
     pp->nDgramPackets = 1;
     pp->congestSeq = 0;
+
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_RESTORE();
+#endif
+
 }
 
 
@@ -624,6 +638,10 @@ rxi_GetIFInfo(void)
     rx_ifnet_t ifn;
 #endif
 
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_SET(TD_TO_VNET(curthread));
+#endif
+
     memset(addrs, 0, sizeof(addrs));
     memset(mtus, 0, sizeof(mtus));
 
@@ -725,6 +743,11 @@ rxi_GetIFInfo(void)
 	    myNetAddrs[l] = addrs[l];
 	}
     }
+
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_RESTORE();
+#endif
+
     return different;
 }
 
