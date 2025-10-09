--- src/afs/afs_server.c.orig	2022-12-15 20:10:23 UTC
+++ src/afs/afs_server.c
@@ -1361,13 +1361,25 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 #   elif defined(AFS_FBSD_ENV)
     {
 	struct in_ifaddr *ifa;
+#   if __FreeBSD_version >= 1400039
+	struct epoch_tracker et;
+#   else
 	struct rm_priotracker in_ifa_tracker;
+#   endif
 	CURVNET_SET(rx_socket->so_vnet);
+#   if __FreeBSD_version >= 1400039
+	NET_EPOCH_ENTER(et);
+#   else
 	IN_IFADDR_RLOCK(&in_ifa_tracker);
+#   endif
 	AFS_FBSD_NET_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
 	    afsi_SetServerIPRank(sa, &ifa->ia_ifa);
 	}
+#   if __FreeBSD_version >= 1400039
+	NET_EPOCH_EXIT(et);
+#   else
 	IN_IFADDR_RUNLOCK(&in_ifa_tracker);
+#   endif
 	CURVNET_RESTORE();
     }
 #   elif defined(AFS_OBSD_ENV)
