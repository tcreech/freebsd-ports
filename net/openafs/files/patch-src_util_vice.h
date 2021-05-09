--- src/util/vice.h.orig	2021-01-14 21:08:41 UTC
+++ src/util/vice.h
@@ -12,7 +12,7 @@
 
 #include <sys/types.h>
 #if !defined(AFS_NT40_ENV)
-# if (!defined(AFS_FBSD80_ENV) && !defined(AFS_LINUX26_ENV)) || !defined(KERNEL) || defined(UKERNEL)
+# if (!defined(AFS_FBSD_ENV) && !defined(AFS_LINUX26_ENV)) || !defined(KERNEL) || defined(UKERNEL)
 #  include <sys/ioctl.h>
 # endif
 #endif /* AFS_NT40_ENV */
