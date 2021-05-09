--- src/vfsck/pass5.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/pass5.c
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
@@ -56,12 +45,11 @@
 #endif
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 #include <afs/osi_inode.h>
 
 #include "fsck.h"
 
-#if	defined(AFS_SUN_ENV) || defined(AFS_OSF_ENV)
+#if	defined(AFS_SUN_ENV)
 #define AFS_NEWCG_ENV
 #else
 #undef AFS_NEWCG_ENV
@@ -156,20 +144,8 @@ pass5()
 	idesc[i].id_type = ADDR;
     memset(&cstotal, 0, sizeof(struct csum));
     (void)time(&now);
-#ifdef notdef
-    /* this is the original from UCB/McKusick, but it is clearly wrong.  It is
-     * rounding the # of fragments to the next 1024 (in our case, with a 1K/8K file system),
-     * while instead it should be rounding to the next block.
-     *
-     * In addition, we should be sure that we allocate enough space, but that seems to be
-     * ensured by the fact that the bitmap is rounded up to the nearest short, and that there
-     * are never more than 16 frags per block.
-     */
-    for (i = fs->fs_size; i < fragroundup(fs, fs->fs_size); i++)
-#else
     c = 1 << fs->fs_fragshift;	/* unit to which we want to round */
     for (i = fs->fs_size; i < roundup(fs->fs_size, c); i++)
-#endif
 	setbmap(i);
     for (c = 0; c < fs->fs_ncg; c++) {
 	getblk(&cgblk, cgtod(fs, c), fs->fs_cgsize);
