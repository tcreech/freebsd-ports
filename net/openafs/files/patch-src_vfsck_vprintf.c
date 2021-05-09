--- src/vfsck/vprintf.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/vprintf.c
@@ -45,17 +45,6 @@
 
 #include <sys/file.h>
 
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
@@ -83,7 +72,6 @@
 #endif
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 
 #include <sys/wait.h>
 #include "fsck.h"
