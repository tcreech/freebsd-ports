--- src/fsint/afsaux.c.orig	2021-01-14 21:08:41 UTC
+++ src/fsint/afsaux.c
@@ -20,7 +20,7 @@
 #if defined(AFS_NBSD50_ENV)
 #include "afs/afs_osi.h"
 #endif
-#if defined(AFS_OSF_ENV) || defined(AFS_LINUX20_ENV) || defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
+#if defined(AFS_LINUX20_ENV) || defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
 #include "afs/sysincludes.h"
 #include "afsincludes.h"
 #else
