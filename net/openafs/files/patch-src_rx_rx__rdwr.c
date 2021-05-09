--- src/rx/rx_rdwr.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_rdwr.c
@@ -24,9 +24,6 @@
 #   if defined(AFS_AIX_ENV) || defined(AFS_AUX_ENV) || defined(AFS_SUN5_ENV)
 #    include "h/systm.h"
 #   endif
-#   ifdef	AFS_OSF_ENV
-#    include <net/net_globals.h>
-#   endif /* AFS_OSF_ENV */
 #   ifdef AFS_LINUX20_ENV
 #    include "h/socket.h"
 #   endif
