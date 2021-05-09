--- src/vfsck/utilities.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/utilities.c
@@ -40,18 +40,6 @@
 #include <ctype.h>
 
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
-#define	AFS_NEWCG_ENV
-#else /* AFS_OSF_ENV */
 #ifdef AFS_VFSINCL_ENV
 #include <sys/vnode.h>
 #ifdef	  AFS_SUN5_ENV
@@ -73,11 +61,10 @@
 #endif
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 
 #include "fsck.h"
 
-#if	defined(AFS_SUN_ENV) || defined(AFS_OSF_ENV)
+#if	defined(AFS_SUN_ENV)
 #define AFS_NEWCG_ENV
 #else
 #undef AFS_NEWCG_ENV
