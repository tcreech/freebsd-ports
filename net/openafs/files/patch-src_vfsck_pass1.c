--- src/vfsck/pass1.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/pass1.c
@@ -23,17 +23,6 @@
 #include <ctype.h>
 
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
@@ -58,7 +47,6 @@ extern int ge_danger;
 #endif
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 
 #include <afs/osi_inode.h>
 #include "fsck.h"
@@ -247,9 +235,6 @@ pass1()
 	    if ((dp->di_mode & IFMT) == IFBLK
 		|| (dp->di_mode & IFMT) == IFCHR)
 		ndb++;
-#ifdef	AFS_OSF_ENV
-	    if ((dp->di_flags & IC_FASTLINK) == 0) {
-#endif /* AFS_OSF_ENV */
 		for (j = ndb; j < NDADDR; j++) {
 #if defined(AFS_HPUX_ENV) && (defined(DUX) || defined(CNODE_DEV))
 		    /*
@@ -284,9 +269,6 @@ pass1()
 #if	defined(AFS_HPUX_ENV)
 	      ignore_direct_block_check:
 #endif
-#ifdef	AFS_OSF_ENV
-	    }
-#endif /* AFS_OSF_ENV */
 	    if (ftypeok(dp) == 0)
 		goto unknown;
 	    n_files++;
