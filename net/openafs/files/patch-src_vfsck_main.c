--- src/vfsck/main.c.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/main.c
@@ -52,12 +52,6 @@
 #undef KERNEL
 #endif
 
-#ifdef	AFS_OSF_ENV
-#include <sys/vnode.h>
-#include <sys/mount.h>
-#include <ufs/inode.h>
-#include <ufs/fs.h>
-#else /* AFS_OSF_ENV */
 #ifdef AFS_VFSINCL_ENV
 #define VFS
 #include <sys/vnode.h>
@@ -85,7 +79,6 @@
 #endif
 #include <sys/fs.h>
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 
 #include <sys/wait.h>
 #ifdef	XAFS_SUN_ENV
@@ -178,7 +171,7 @@ main(argc, argv)
 #if	defined(AFS_HPUX_ENV)
     pclean = 0;
 #endif
-#if	defined(AFS_HPUX_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_OSF_ENV)
+#if	defined(AFS_HPUX_ENV) || defined(AFS_SUN5_ENV)
     fflag = 0;
 #endif
 #ifdef	AFS_SUN5_ENV
@@ -334,11 +327,6 @@ main(argc, argv)
 	    break;
 #endif /* AFS_HPUX100_ENV */
 #endif /* AFS_SUN5_ENV */
-#ifdef	AFS_OSF_ENV
-	case 'o':
-	    fflag++;
-	    break;
-#endif /* AFS_OSF_ENV */
 	case 'n':
 	case 'N':
 	    nflag++;
@@ -435,11 +423,6 @@ main(argc, argv)
 		&& strcmp(fsp->fs_type, FSTAB_RO)
 		&& strcmp(fsp->fs_type, FSTAB_RQ))
 		continue;
-#ifdef	AFS_OSF_ENV
-	    if (strcmp(fsp->fs_vfstype, "ufs") || fsp->fs_passno == 0) {
-		continue;
-	    }
-#endif /* AFS_OSF_ENV */
 	    if (preen == 0 || passno == 1 && fsp->fs_passno == 1) {
 		if (passno == 1) {
 		    name = blockcheck(fsp->fs_spec);
@@ -618,10 +601,6 @@ checkfilesys(filesys, parname)
     char devbuffer[128];
     int ret_val;
 
-#ifdef	AFS_OSF_ENV
-    int temp;
-#endif /* AFS_OSF_ENV */
-
 #ifdef	AFS_SUN_ENV
     iscorrupt = 1;
 #endif
@@ -660,10 +639,6 @@ checkfilesys(filesys, parname)
 	} else if (ret_val == -1) {	/* pclean && FS_CLEAN */
 	    return (1);
 #endif
-#if	defined(AFS_OSF_ENV)
-	} else if (ret_val == FS_CLEAN) {	/* pclean && FS_CLEAN */
-	    return (1);
-#endif
 	}
 #if	defined(AFS_HPUX100_ENV)
 	if (mflag)
@@ -874,22 +849,11 @@ checkfilesys(filesys, parname)
 	sbdirty();
     }
 #else
-#ifdef	AFS_OSF_ENV
-    if (!nflag && !bflag && !hotroot) {
-	temp = fsmodified;
-	sblock.fs_clean = FS_CLEAN;
-	(void)time(&sblock.fs_time);
-	sbdirty();
-	flush(fswritefd, &sblk);
-	fsmodified = temp;
-    }
-#else /* AFS_OSF_ENV */
     if (fsmodified) {
 	(void)time(&sblock.fs_time);
 	sbdirty();
     }
 #endif
-#endif
     ckfini();
     free(blockmap);
     free(statemap);
@@ -958,18 +922,6 @@ checkfilesys(filesys, parname)
 	    }
 	}
 #endif
-#ifdef	AFS_OSF_ENV
-	/* if system mounted / as read-only, we'll try to fix now */
-	if (access("/", W_OK) < 0 && errno == EROFS) {
-	    printf("Can't RW acceess /; %d\n", errno);
-	    code = system("/sbin/mount -u /");
-	    if (code) {
-		printf("Couldn't remount / R/W; continuing anyway (%d).\n",
-		       errno);
-		failed = 1;
-	    }
-	}
-#endif
 	rmdir(pname);
 	unlink(pname);
 	if (mkdir(pname, 0777) < 0) {
@@ -1010,15 +962,11 @@ checkfilesys(filesys, parname)
 		fstat(fd, &tstat);
 		close(fd);
 	    }
-#if !defined(AFS_HPUX_ENV) && !defined(AFS_SUN5_ENV) && !defined(AFS_OSF_ENV)
+#if !defined(AFS_HPUX_ENV) && !defined(AFS_SUN5_ENV)
 	    unmount(pname);
 #else
-#if	defined(AFS_OSF_ENV)
-	    umount(pname, MNT_NOFORCE);
-#else /* AFS_OSF_ENV */
 	    umount(devname);
 #endif
-#endif
 	}
 	rmdir(pname);
     }
@@ -1054,9 +1002,7 @@ blockcheck(name)
     if ((stblock.st_mode & S_IFMT) == S_IFBLK) {
 	if (stslash.st_dev == stblock.st_rdev) {
 	    hotroot++;
-#if	!defined(AFS_OSF_ENV)	/*  OSF/1 always uses the raw device, even for / */
 	    return (name);
-#endif /* AFS_OSF_ENV */
 	}
 	raw = rawname(name);
 	if (raw) {
