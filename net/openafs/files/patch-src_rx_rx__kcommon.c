--- src/rx/rx_kcommon.c.orig	2018-09-11 15:52:48 UTC
+++ src/rx/rx_kcommon.c
@@ -22,6 +22,11 @@
 #include "rx_stats.h"
 #include "rx_peer.h"
 
+#if defined(AFS_FBSD80_ENV)
+#include <sys/param.h>
+#include <sys/jail.h>
+#endif
+
 #ifdef AFS_HPUX110_ENV
 #include "h/tihdr.h"
 #include <xti.h>
@@ -388,6 +393,13 @@ rxi_InitPeerParams(struct rx_peer *pp)
     }
 # else /* AFS_USERSPACE_IP_ADDR */
     rx_ifnet_t ifn;
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_SET(TD_TO_VNET(curthread));
+    IFNET_RLOCK();
+#if defined(AFS_FBSD120_ENV)
+    NET_EPOCH_ENTER();
+#endif
+#endif
 
 #  if !defined(AFS_SGI62_ENV)
     if (numMyNetAddrs == 0)
@@ -451,6 +463,13 @@ rxi_InitPeerParams(struct rx_peer *pp)
     pp->cwind = 1;
     pp->nDgramPackets = 1;
     pp->congestSeq = 0;
+#if defined(AFS_FBSD80_ENV)
+#if defined(AFS_FBSD120_ENV)
+    NET_EPOCH_EXIT();
+#endif
+    IFNET_RUNLOCK();
+    CURVNET_RESTORE();
+#endif
 }
 
 
@@ -658,7 +677,9 @@ rxi_GetIFInfo(void)
     }
 #else
 #if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
-#if defined(AFS_FBSD80_ENV)
+#if defined(AFS_FBSD120_ENV)
+    CK_STAILQ_FOREACH(ifn, &V_ifnet, if_link) {
+#elif defined(AFS_FBSD80_ENV)
     TAILQ_FOREACH(ifn, &V_ifnet, if_link) {
 #else
     TAILQ_FOREACH(ifn, &ifnet, if_link) {
@@ -673,7 +694,14 @@ rxi_GetIFInfo(void)
 #endif
 	rxmtu = (ifn->if_mtu - RX_IPUDP_SIZE);
 #if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
+#if defined(AFS_FBSD120_ENV)
+	CK_STAILQ_FOREACH(ifad, &ifn->if_addrhead, ifa_link) {
+#else
 	TAILQ_FOREACH(ifad, &ifn->if_addrhead, ifa_link) {
+#endif
+#if defined(AFS_FBSD80_ENV)
+            IF_ADDR_RLOCK(ifn);
+#endif
 	    if (i >= ADDRSPERSITE)
 		break;
 #elif defined(AFS_OBSD_ENV) || defined(AFS_NBSD_ENV)
@@ -704,6 +732,9 @@ rxi_GetIFInfo(void)
 			MIN(rx_maxReceiveSize, rx_maxReceiveSizeUser);
 		}
 	    }
+#if defined(AFS_FBSD80_ENV)
+            IF_ADDR_RUNLOCK(ifn);
+#endif
 	}
     }
 #endif
@@ -733,6 +764,7 @@ rxi_FindIfnet(afs_uint32 addr, afs_uint3
 
     s.sin_family = AF_INET;
     s.sin_addr.s_addr = addr;
+
     ifad = rx_ifaddr_withnet((struct sockaddr *)&s);
 
     if (ifad && maskp) {
