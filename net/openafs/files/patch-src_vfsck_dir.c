--- src/vfsck/dir.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/dir.c
@@ -40,17 +40,6 @@
 
 #define VICE			/* allow us to put our changes in at will */
 
-#ifdef	AFS_OSF_ENV
-#include <sys/vnode.h>
-#include <sys/mount.h>
-#include <ufs/inode.h>
-#include <ufs/fs.h>
-#define	_BSD
-#define	_KERNEL
-#include <ufs/dir.h>
-#undef	_KERNEL
-#undef	_BSD
-#else /* AFS_OSF_ENV */
 #ifdef AFS_VFSINCL_ENV
 #define VFS
 #include <sys/vnode.h>
@@ -77,7 +66,6 @@
 #endif
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 
 #ifdef AFS_SUN_ENV
 #ifdef	AFS_SUN5_ENV
