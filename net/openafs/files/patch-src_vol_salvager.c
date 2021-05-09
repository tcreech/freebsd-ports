--- src/vol/salvager.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/salvager.c
@@ -51,12 +51,8 @@
 #endif
 #endif
 #else /* AFS_VFSINCL_ENV */
-#ifdef	AFS_OSF_ENV
-#include <ufs/inode.h>
-#else /* AFS_OSF_ENV */
 #if !defined(AFS_LINUX20_ENV) && !defined(AFS_XBSD_ENV) && !defined(AFS_DARWIN_ENV)
 #include <sys/inode.h>
-#endif
 #endif
 #endif /* AFS_VFSINCL_ENV */
 #endif /* AFS_SGI_ENV */
