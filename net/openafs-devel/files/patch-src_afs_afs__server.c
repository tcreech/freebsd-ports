--- src/afs/afs_server.c.orig	2019-08-30 16:11:36 UTC
+++ src/afs/afs_server.c
@@ -1354,21 +1354,18 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 	    TAILQ_FOREACH(ifa, &ifn->if_addrhead, ifa_link) {
 		afsi_SetServerIPRank(sa, ifa);
     }}}
-#elif defined(AFS_FBSD80_ENV)
+#elif defined(AFS_FBSD_ENV)
     {
 	struct in_ifaddr *ifa;
 	CURVNET_SET(rx_socket->so_vnet);
+#if defined(AFS_FBSD120_ENV)
+	CK_STAILQ_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
+#else
 	TAILQ_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
+#endif
 	    afsi_SetServerIPRank(sa, &ifa->ia_ifa);
 	}
 	CURVNET_RESTORE();
-    }
-#elif defined(AFS_FBSD_ENV)
-    {
-	struct in_ifaddr *ifa;
-	TAILQ_FOREACH(ifa, &in_ifaddrhead, ia_link) {
-	    afsi_SetServerIPRank(sa, &ifa->ia_ifa);
-	}
     }
 #elif defined(AFS_OBSD_ENV)
     {
