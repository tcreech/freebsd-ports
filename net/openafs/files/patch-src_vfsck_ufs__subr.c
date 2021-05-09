--- src/vfsck/ufs_subr.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/ufs_subr.c
@@ -24,9 +24,6 @@
 
 #define VICE
 
-#ifdef	AFS_OSF_ENV
-#include <ufs/fs.h>
-#else /* AFS_OSF_ENV */
 #ifdef AFS_VFSINCL_ENV
 #ifdef	AFS_SUN5_ENV
 #include <sys/fs/ufs_fs.h>
@@ -36,7 +33,6 @@
 #else /* AFS_VFSINCL_ENV */
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 
 extern int around[9];
 extern int inside[9];
