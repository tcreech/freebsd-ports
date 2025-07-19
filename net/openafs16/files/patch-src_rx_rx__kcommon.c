--- src/rx/rx_kcommon.c.orig	2018-09-11 15:54:34 UTC
+++ src/rx/rx_kcommon.c
@@ -17,6 +17,11 @@
 
 #include "rx/rx_kcommon.h"
 
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
 #  else /* AFS_USERSPACE_IP_ADDR */
     rx_ifnet_t ifn;
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_SET(TD_TO_VNET(curthread));
+    IFNET_RLOCK();
+#if defined(AFS_FBSD120_ENV)
+    NET_EPOCH_ENTER();
+#endif
+#endif
 
 #   if !defined(AFS_SGI62_ENV)
     if (numMyNetAddrs == 0)
@@ -456,6 +468,13 @@ rxi_InitPeerParams(struct rx_peer *pp)
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
 
 
@@ -663,7 +682,9 @@ rxi_GetIFInfo(void)
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
@@ -678,7 +699,14 @@ rxi_GetIFInfo(void)
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
@@ -709,6 +737,9 @@ rxi_GetIFInfo(void)
 			MIN(rx_maxReceiveSize, rx_maxReceiveSizeUser);
 		}
 	    }
+#if defined(AFS_FBSD80_ENV)
+            IF_ADDR_RUNLOCK(ifn);
+#endif
 	}
     }
 #endif
@@ -738,6 +769,7 @@ rxi_FindIfnet(afs_uint32 addr, afs_uint32 * maskp)
 
     s.sin_family = AF_INET;
     s.sin_addr.s_addr = addr;
+
     ifad = rx_ifaddr_withnet((struct sockaddr *)&s);
 
     if (ifad && maskp) {
