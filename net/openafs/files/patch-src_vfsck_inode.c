--- src/vfsck/inode.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/inode.c
@@ -40,17 +40,6 @@
 
 #define VICE			/* control whether AFS changes are present */
 
-#ifdef	AFS_OSF_ENV
-#include <sys/mount.h>
-#include <sys/vnode.h>
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
@@ -74,7 +63,6 @@
 #endif
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 
 #include <afs/osi_inode.h>
 #include "fsck.h"
@@ -109,24 +97,9 @@ ckinode(dp, idesc)
     idesc->id_filesize = dp->di_size;
     if ((dp->di_mode & IFMT) == IFBLK || (dp->di_mode & IFMT) == IFCHR)
 	return (KEEPON);
-#ifdef	AFS_OSF_ENV
-    if ((dp->di_flags & IC_FASTLINK) != 0) {
-	return (KEEPON);
-    }
-#endif /* AFS_OSF_ENV */
     dino = *dp;
     ndb = howmany(dino.di_size, (UOFF_T) sblock.fs_bsize);
     ap = &dino.di_db[0];
-#ifdef AFS_OSF_ENV
-    /*
-     * Check property lists on pass1
-     */
-    if (idesc->id_func == pass1check && dino.di_flags & IC_PROPLIST && *ap) {
-	ret = proplist_scan(dp, idesc);
-	if (ret & STOP)
-	    return (ret);
-    }
-#endif /* AFS_OSF_ENV */
     for (; ap < &dino.di_db[NDADDR]; ap++) {
 	if (--ndb == 0 && (offset = blkoff(&sblock, dino.di_size)) != 0)
 	    idesc->id_numfrags =
