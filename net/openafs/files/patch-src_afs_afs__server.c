--- src/afs/afs_server.c.orig	2025-01-23 17:12:55 UTC
+++ src/afs/afs_server.c
@@ -51,6 +51,9 @@
 # ifdef AFS_DARWIN_ENV
 #  include <net/if_var.h>
 # endif
+# ifdef AFS_FBSD_ENV
+#  include <net/if_private.h>
+# endif
 #endif /* !defined(UKERNEL) */
 
 #include "afsincludes.h"	/* Afs-based standard headers */
@@ -1361,13 +1364,25 @@ afs_SetServerPrefs(struct srvAddr *const sa)
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
