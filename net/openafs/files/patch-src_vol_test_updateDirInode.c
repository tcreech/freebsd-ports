--- src/vol/test/updateDirInode.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/test/updateDirInode.c
@@ -23,9 +23,6 @@
 #include <ctype.h>
 #include <sys/param.h>
 #if !defined(AFS_SGI_ENV)
-#ifdef	AFS_OSF_ENV
-#include <ufs/fs.h>
-#else /* AFS_OSF_ENV */
 #ifdef AFS_VFSINCL_ENV
 #define VFS
 #ifdef	AFS_SUN5_ENV
@@ -38,7 +35,6 @@
 #include <sys/fs.h>
 #endif
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 #endif /* AFS_SGI_ENV */
 #include <sys/errno.h>
 #include <sys/stat.h>
@@ -106,9 +102,6 @@
 #include <afs/dir.h>
 #include <unistd.h>
 
-#ifdef	AFS_OSF_ENV
-extern void *calloc(), *realloc();
-#endif
 #include "salvage.h"
 int volumeId;
 int VolumeChanged;		/* to satisfy library libdir use */
