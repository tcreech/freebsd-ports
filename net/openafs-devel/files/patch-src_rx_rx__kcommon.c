--- src/rx/rx_kcommon.c.orig	2019-08-30 16:11:36 UTC
+++ src/rx/rx_kcommon.c
@@ -658,9 +658,13 @@ rxi_GetIFInfo(void)
     }
 #else
 #if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
-#if defined(AFS_FBSD80_ENV)
+#if defined(AFS_FBSD_ENV)
     CURVNET_SET(rx_socket->so_vnet);
+#if defined(AFS_FBSD120_ENV)
+    CK_STAILQ_FOREACH(ifn, &V_ifnet, if_link) {
+#else
     TAILQ_FOREACH(ifn, &V_ifnet, if_link) {
+#endif
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
@@ -722,7 +736,7 @@ rxi_GetIFInfo(void)
 	}
     }
 
-#ifdef AFS_FBSD80_ENV
+#ifdef AFS_FBSD_ENV
     CURVNET_RESTORE();
 #endif
 
@@ -738,9 +752,13 @@ rxi_FindIfnet(afs_uint32 addr, afs_uint32 * maskp)
     rx_ifaddr_t ifad;
     rx_ifnet_t ret;
 
-#ifdef AFS_FBSD80_ENV
+#ifdef AFS_FBSD_ENV
     CURVNET_SET(rx_socket->so_vnet);
+    IFNET_RLOCK();
+#ifdef AFS_FBSD120_ENV
+    NET_EPOCH_ENTER();
 #endif
+#endif
 
     s.sin_family = AF_INET;
     s.sin_addr.s_addr = addr;
@@ -753,7 +771,11 @@ rxi_FindIfnet(afs_uint32 addr, afs_uint32 * maskp)
 
     ret = (ifad ? rx_ifaddr_ifnet(ifad) : NULL);
 
-#ifdef AFS_FBSD80_ENV
+#ifdef AFS_FBSD_ENV
+#ifdef AFS_FBSD120_ENV
+    NET_EPOCH_EXIT();
+#endif
+    IFNET_RUNLOCK();
     CURVNET_RESTORE();
 #endif
 
