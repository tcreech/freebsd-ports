--- src/afs/afs_server.c.orig	2020-03-01 19:58:46 UTC
+++ src/afs/afs_server.c
@@ -1358,7 +1358,11 @@ afs_SetServerPrefs(struct srvAddr *const sa)
     {
 	struct in_ifaddr *ifa;
 	CURVNET_SET(rx_socket->so_vnet);
+#if defined(AFS_FBSD120_ENV)
+	CK_STAILQ_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
+#else
 	TAILQ_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
+#endif /* AFS_FBSD120_ENV */
 	    afsi_SetServerIPRank(sa, &ifa->ia_ifa);
 	}
 	CURVNET_RESTORE();
