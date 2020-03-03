--- src/rx/rx_kcommon.c.orig	2020-02-21 20:17:12 UTC
+++ src/rx/rx_kcommon.c
@@ -660,7 +660,11 @@ rxi_GetIFInfo(void)
 #if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
 #if defined(AFS_FBSD_ENV)
     CURVNET_SET(rx_socket->so_vnet);
+#if defined(AFS_FBSD120_ENV)
+    CK_STAILQ_FOREACH(ifn, &V_ifnet, if_link) {
+#else
     TAILQ_FOREACH(ifn, &V_ifnet, if_link) {
+#endif /* AFS_FBSD120_ENV */
 #else
     TAILQ_FOREACH(ifn, &ifnet, if_link) {
 #endif
@@ -674,7 +678,14 @@ rxi_GetIFInfo(void)
 #endif
 	rxmtu = (ifn->if_mtu - RX_IPUDP_SIZE);
 #if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
+#if defined(AFS_FBSD120_ENV)
+	CK_STAILQ_FOREACH(ifad, &ifn->if_addrhead, ifa_link) {
+#else
 	TAILQ_FOREACH(ifad, &ifn->if_addrhead, ifa_link) {
+#endif
+#if defined(AFS_FBSD_ENV)
+            IF_ADDR_RLOCK(ifn);
+#endif
 	    if (i >= ADDRSPERSITE)
 		break;
 #elif defined(AFS_OBSD_ENV) || defined(AFS_NBSD_ENV)
@@ -705,6 +716,9 @@ rxi_GetIFInfo(void)
 			MIN(rx_maxReceiveSize, rx_maxReceiveSizeUser);
 		}
 	    }
+#if defined(AFS_FBSD_ENV)
+            IF_ADDR_RUNLOCK(ifn);
+#endif
 	}
     }
 #endif
