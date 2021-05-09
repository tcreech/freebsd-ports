--- src/vfsck/setup.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/setup.c
@@ -24,7 +24,7 @@
 
 #define VICE
 
-#if	defined(AFS_SUN_ENV) || defined(AFS_OSF_ENV)
+#if	defined(AFS_SUN_ENV)
 #define AFS_NEWCG_ENV
 #else
 #undef AFS_NEWCG_ENV
@@ -37,17 +37,6 @@ extern vfscklogprintf();
 
 #define DKTYPENAMES
 
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
@@ -70,7 +59,6 @@ extern vfscklogprintf();
 #endif
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 
 #include <sys/file.h>
 #ifdef	AFS_SUN5_ENV
@@ -552,13 +540,6 @@ setup(dev)
 	return (0);
     }
 #endif
-#ifdef	AFS_OSF_ENV
-    if (!fflag && !bflag && !nflag && !hotroot && sblock.fs_clean == FS_CLEAN
-	&& !sblk.b_dirty) {
-	pwarn("Clean file system - skipping fsck\n");
-	return (FS_CLEAN);
-    }
-#endif /* AFS_OSF_ENV */
 
     /*
      * allocate and initialize the necessary maps
