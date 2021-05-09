--- src/vfsck/pass3.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/pass3.c
@@ -24,17 +24,6 @@
 
 #define VICE
 
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
 #include <sys/vnode.h>
 #ifdef	  AFS_SUN5_ENV
@@ -57,7 +46,6 @@
 #endif
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 
 #include <afs/osi_inode.h>
 #include "fsck.h"
