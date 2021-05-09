--- src/config/param.nbsd40.h.orig	2021-01-14 21:08:41 UTC
+++ src/config/param.nbsd40.h
@@ -10,14 +10,6 @@
 #define AFS_MOUNT_AFS "afs"	/* The name of the filesystem type. */
 #define AFS_SYSCALL 318 /* 210 */
 
-#if 0
-/* including this file before sysincludes.h is canonical, but
- * NBSD40's mount.h defines MOUNT_AFS */
-#ifndef	MOUNT_AFS
-#define	MOUNT_AFS AFS_MOUNT_AFS
-#endif
-#endif
-
 #define AFS_XBSD_ENV 1		/* {Free,Open,Net}BSD */
 
 #define AFS_NBSD_ENV 1
