--- src/afs/afs_server.c.orig	2016-12-08 04:01:51 UTC
+++ src/afs/afs_server.c
@@ -42,6 +42,11 @@
 #endif
 #include <netinet/in.h>
 
+#if defined(AFS_FBSD80_ENV)
+#include <sys/param.h>
+#include <sys/jail.h>
+#endif
+
 #ifdef AFS_SGI62_ENV
 #include "h/hashing.h"
 #endif
@@ -1351,6 +1356,9 @@ afs_SetServerPrefs(struct srvAddr *const
 		afsi_SetServerIPRank(sa, ifa);
     }}}
 #elif defined(AFS_FBSD_ENV)
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_SET(TD_TO_VNET(curthread));
+#endif
     {
 	struct in_ifaddr *ifa;
 #if defined(AFS_FBSD80_ENV)
@@ -1388,6 +1396,9 @@ afs_SetServerPrefs(struct srvAddr *const
 #endif				/* AFS_SUN5_ENV */
 #endif				/* else AFS_USERSPACE_IP_ADDR */
     sa->sa_iprank += afs_randomMod15();
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_RESTORE();
+#endif
 
     return 0;
 }				/* afs_SetServerPrefs */
