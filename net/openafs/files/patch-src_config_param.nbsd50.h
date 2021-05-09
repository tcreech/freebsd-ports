--- src/config/param.nbsd50.h.orig	2021-01-14 21:08:41 UTC
+++ src/config/param.nbsd50.h
@@ -15,16 +15,6 @@
 #define AFS_KFREE               kmem_free
 #define VATTR_NULL              vattr_null
 
-#if 0
-/* including this file before sysincludes.h is canonical, but
- * NBSD40's mount.h defines MOUNT_AFS */
-
-#ifndef	MOUNT_AFS
-#define	MOUNT_AFS AFS_MOUNT_AFS
-#endif
-
-#endif /* 0 */
-
 #define AFS_XBSD_ENV 1		/* {Free,Open,Net}BSD */
 
 #define AFS_NBSD_ENV 1
