--- src/rx/rx_kcommon.h.orig	2025-01-23 17:12:55 UTC
+++ src/rx/rx_kcommon.h
@@ -122,6 +122,7 @@ struct coda_inode_info {
 #  include <sys/socket.h>
 #  include <net/if.h>
 #  include <net/if_var.h>
+#  include <net/if_private.h>
 # endif
 #endif
 #if !defined(AFS_HPUX110_ENV) && !defined(AFS_LINUX_ENV) && !defined(AFS_DARWIN_ENV)
