--- src/gtx/curseswindows.c.orig	2021-01-14 21:08:41 UTC
+++ src/gtx/curseswindows.c
@@ -20,7 +20,7 @@
 
 #include <roken.h>
 
-#if !defined(AFS_SUN5_ENV) && !defined(AFS_LINUX20_ENV) && !defined(AFS_FBSD80_ENV)
+#if !defined(AFS_SUN5_ENV) && !defined(AFS_LINUX20_ENV) && !defined(AFS_FBSD_ENV)
 #include <sgtty.h>
 #endif
 
