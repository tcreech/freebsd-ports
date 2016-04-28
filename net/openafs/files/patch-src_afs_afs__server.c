--- src/afs/afs_server.c.orig	2016-04-23 02:20:17 UTC
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
@@ -1642,6 +1647,9 @@ afs_SetServerPrefs(struct srvAddr *sa)
 		afsi_SetServerIPRank(sa, ifa);
     }}}
 #elif defined(AFS_FBSD_ENV)
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_SET(TD_TO_VNET(curthread));
+#endif
     {
 	struct in_ifaddr *ifa;
 #if defined(AFS_FBSD80_ENV)
@@ -1681,6 +1689,10 @@ afs_SetServerPrefs(struct srvAddr *sa)
     if (sa)
 	  sa->sa_iprank += afs_randomMod15();
 
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_RESTORE();
+#endif
+
     return 0;
 }				/* afs_SetServerPrefs */
 
