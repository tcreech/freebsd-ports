--- src/vol/listinodes.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/listinodes.c
@@ -44,9 +44,6 @@ ListViceInodes(char *devname, char *mountedOn, FD_t in
 }
 #else
 #if !defined(AFS_SGI_ENV)
-#ifdef	AFS_OSF_ENV
-#include <ufs/fs.h>
-#else /* AFS_OSF_ENV */
 #ifdef AFS_VFSINCL_ENV
 #define VFS
 #ifdef	  AFS_SUN5_ENV
@@ -67,7 +64,6 @@ ListViceInodes(char *devname, char *mountedOn, FD_t in
 #include <sys/fs.h>
 #endif
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 #ifdef AFS_VFSINCL_ENV
 #include <sys/vnode.h>
 #ifdef	  AFS_SUN5_ENV
@@ -78,11 +74,7 @@ ListViceInodes(char *devname, char *mountedOn, FD_t in
 #endif
 #endif
 #else /* AFS_VFSINCL_ENV */
-#ifdef	AFS_OSF_ENV
-#include <ufs/inode.h>
-#else /* AFS_OSF_ENV */
 #include <sys/inode.h>
-#endif
 #endif /* AFS_VFSINCL_ENV */
 #endif /* AFS_SGI_ENV */
 #include <afs/osi_inode.h>
@@ -1001,7 +993,7 @@ ListViceInodes(char *devname, char *mountedOn, FD_t in
 #else
     if ((super.fs.fs_magic != FS_MAGIC)
 	|| (super.fs.fs_ncg < 1)
-#if	defined(AFS_SUN_ENV) || defined(AFS_OSF_ENV) || defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
+#if	defined(AFS_SUN_ENV) || defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
 	|| (super.fs.fs_cpg < 1)
 #else
 	|| (super.fs.fs_cpg < 1 || super.fs.fs_cpg > MAXCPG)
@@ -1050,10 +1042,6 @@ ListViceInodes(char *devname, char *mountedOn, FD_t in
 #endif /* AFS_SUN5_ENV */
 	i = c * super.fs.fs_ipg;
 	e = i + super.fs.fs_ipg;
-#ifdef	AFS_OSF_ENV
-	dblk1 = fsbtodb(&super.fs, itod(&super.fs, i));
-	if (OS_SEEK(pfd, (off_t) ((off_t) dblk1 * DEV_BSIZE), L_SET) == -1) {
-#else
 #if defined(AFS_SUN5_ENV) || defined(AFS_DARWIN_ENV)
 	f1 = fsbtodb(&super.fs, itod(&super.fs, i));
 	off = (offset_t) f1 << DEV_BSHIFT;
@@ -1062,7 +1050,6 @@ ListViceInodes(char *devname, char *mountedOn, FD_t in
 	if (OS_SEEK(pfd, dbtob(fsbtodb(&super.fs, itod(&super.fs, i))), L_SET)
 	    == -1) {
 #endif /* AFS_SUN5_ENV */
-#endif /* AFS_OSF_ENV */
 #endif /* AFS_HPUX_ENV */
 	    Log("Error reading inodes for partition %s; run vfsck\n",
 		partition);
@@ -1098,27 +1085,6 @@ ListViceInodes(char *devname, char *mountedOn, FD_t in
 		}
 	    }
 	    for (p = inodes; p < einodes && i < e; i++, p++) {
-#ifdef notdef
-		Log("Ino=%d, v1=%x, v2=%x, v3=%x, mode=%x size=%d, lcnt=%d\n",
-		    i, p->di_vicep1, p->di_vicep2, p->di_vicep3, p->di_mode,
-		    p->di_size, p->di_nlink);
-		printf
-		    ("Ino=%d, v1=%x, v2=%x, v3=%x, mode=%x size=%d, lcnt=%d\n",
-		     i, p->di_vicep1, p->di_vicep2, p->di_vicep3, p->di_mode,
-		     p->di_size, p->di_nlink);
-#endif
-#ifdef AFS_OSF_ENV
-#ifdef AFS_3DISPARES
-		/* Check to see if this inode is a pre-"OSF1 4.0D" inode */
-		if ((p->di_uid || p->di_gid)
-		    && !(p->di_flags & (IC_XUID | IC_XGID))) {
-		    Log("Found unconverted inode %d: Use 'fs_conv_dux40D convert' on partition %s\n", i, partition);
-		    goto out;
-		}
-#else
-		osi_Panic("Tru64 needs AFS_3DISPARES\n");
-#endif
-#endif
 #if	defined(AFS_SUN5_ENV)
 		/* if this is a pre-sol2.6 unconverted inode, bail out */
 		{
