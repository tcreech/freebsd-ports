--- src/afs/afs_server.c.orig	2022-12-15 20:10:23 UTC
+++ src/afs/afs_server.c
@@ -1361,13 +1361,13 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 #   elif defined(AFS_FBSD_ENV)
     {
 	struct in_ifaddr *ifa;
-	struct rm_priotracker in_ifa_tracker;
+	//struct rm_priotracker in_ifa_tracker;
 	CURVNET_SET(rx_socket->so_vnet);
-	IN_IFADDR_RLOCK(&in_ifa_tracker);
+	//IN_IFADDR_RLOCK(&in_ifa_tracker);
 	AFS_FBSD_NET_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
 	    afsi_SetServerIPRank(sa, &ifa->ia_ifa);
 	}
-	IN_IFADDR_RUNLOCK(&in_ifa_tracker);
+	//IN_IFADDR_RUNLOCK(&in_ifa_tracker);
 	CURVNET_RESTORE();
     }
 #   elif defined(AFS_OBSD_ENV)
