--- src/afs/afs_server.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/afs_server.c
@@ -1361,10 +1361,13 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 #   elif defined(AFS_FBSD_ENV)
     {
 	struct in_ifaddr *ifa;
+	struct rm_priotracker in_ifa_tracker;
 	CURVNET_SET(rx_socket->so_vnet);
-	TAILQ_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
+	IN_IFADDR_RLOCK(&in_ifa_tracker);
+	AFS_FBSD_NET_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
 	    afsi_SetServerIPRank(sa, &ifa->ia_ifa);
 	}
+	IN_IFADDR_RUNLOCK(&in_ifa_tracker);
 	CURVNET_RESTORE();
     }
 #   elif defined(AFS_OBSD_ENV)
