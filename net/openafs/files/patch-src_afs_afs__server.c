--- src/afs/afs_server.c.orig	2018-09-11 15:52:48 UTC
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
@@ -1351,9 +1356,14 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 		afsi_SetServerIPRank(sa, ifa);
     }}}
 #elif defined(AFS_FBSD_ENV)
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_SET(TD_TO_VNET(curthread));
+#endif
     {
 	struct in_ifaddr *ifa;
-#if defined(AFS_FBSD80_ENV)
+#if defined(AFS_FBSD120_ENV)
+	  CK_STAILQ_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
+#elif defined(AFS_FBSD80_ENV)
 	  TAILQ_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
 #else
 	  TAILQ_FOREACH(ifa, &in_ifaddrhead, ia_link) {
@@ -1388,6 +1398,9 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 #endif				/* AFS_SUN5_ENV */
 #endif				/* else AFS_USERSPACE_IP_ADDR */
     sa->sa_iprank += afs_randomMod15();
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_RESTORE();
+#endif
 
     return 0;
 }				/* afs_SetServerPrefs */
