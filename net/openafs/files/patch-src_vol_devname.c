--- src/vol/devname.c.orig	2021-07-29 10:24:31 UTC
+++ src/vol/devname.c
@@ -10,6 +10,8 @@
 #include <afsconfig.h>
 #include <afs/param.h>
 
+#ifndef AFS_NAMEI_ENV
+
 #include <roken.h>
 
 #include <ctype.h>
@@ -19,51 +21,45 @@
 #include <afs/afsint.h>
 
 #if !defined(AFS_SGI_ENV)
-#ifdef AFS_VFSINCL_ENV
-#define VFS
-#ifdef	AFS_SUN5_ENV
-#include <sys/fs/ufs_fs.h>
-#else
-#if defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
-#include <ufs/ufs/dinode.h>
-#include <ufs/ffs/fs.h>
-#else
-#include <ufs/fs.h>
-#endif
-#endif
-#else /* AFS_VFSINCL_ENV */
-#if !defined(AFS_AIX_ENV) && !defined(AFS_LINUX20_ENV) && !defined(AFS_XBSD_ENV) && !defined(AFS_DARWIN_ENV)
-#include <sys/fs.h>
-#endif
-#endif /* AFS_VFSINCL_ENV */
+# ifdef AFS_VFSINCL_ENV
+#  define VFS
+#  ifdef	AFS_SUN5_ENV
+#   include <sys/fs/ufs_fs.h>
+#  else
+#   include <ufs/fs.h>
+#  endif
+# else /* AFS_VFSINCL_ENV */
+#  if !defined(AFS_AIX_ENV)
+#   include <sys/fs.h>
+#  endif
+# endif /* AFS_VFSINCL_ENV */
 #endif /* AFS_SGI_ENV */
+
 #include <sys/file.h>
+
 #ifdef	AFS_AIX_ENV
-#include <sys/vfs.h>
+# include <sys/vfs.h>
 #else
-#ifdef	AFS_HPUX_ENV
-#include <mntent.h>
-#else
-#if	defined(AFS_SUN_ENV) || defined(AFS_SUN5_ENV)
-#ifdef	AFS_SUN5_ENV
-#include <sys/mnttab.h>
-#include <sys/mntent.h>
-#else
-#include <mntent.h>
+# ifdef	AFS_HPUX_ENV
+#  include <mntent.h>
+# else
+#  if	defined(AFS_SUN_ENV) || defined(AFS_SUN5_ENV)
+#   ifdef	AFS_SUN5_ENV
+#    include <sys/mnttab.h>
+#    include <sys/mntent.h>
+#   else
+#    include <mntent.h>
+#   endif
+#  else
+#   if defined(AFS_SGI_ENV)
+#    include <mntent.h>
+#   else
+#    include <fstab.h>
+#   endif
+#  endif /* AFS_SGI_ENV */
+# endif /* AFS_HPUX_ENV */
 #endif
-#else
-#if defined(AFS_SGI_ENV)
-#include <mntent.h>
-#else
-#ifdef AFS_LINUX22_ENV
-#include <mntent.h>
-#else
-#include <fstab.h>
-#endif
-#endif
-#endif /* AFS_SGI_ENV */
-#endif /* AFS_HPUX_ENV */
-#endif
+
 #include <sys/wait.h>
 #include <setjmp.h>
 
@@ -82,13 +78,14 @@ vol_DevName(dev_t adev, char *wpath)
     struct mnttab mnt;
     FILE *mntfile;
 #else
-#if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV) || defined(AFS_LINUX22_ENV)
+# if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV)
     struct mntent *mntent;
     FILE *mfd;
-#else
+# else
     struct fstab *fsent;
-#endif
-#endif
+# endif
+#endif /* AFS_SUN5_ENV */
+
 #ifdef	AFS_AIX_ENV
     int nmounts;
     struct vmount *vmountp;
@@ -103,59 +100,46 @@ vol_DevName(dev_t adev, char *wpath)
 	 (struct vmount *)((int)vmountp + vmountp->vmt_length)) {
 	char *part = vmt2dataptr(vmountp, VMT_STUB);
 #else
-#ifdef	AFS_SUN5_ENV
+# ifdef	AFS_SUN5_ENV
     if (!(mntfile = fopen(MNTTAB, "r"))) {
 	return NULL;
     }
     while (!getmntent(mntfile, &mnt)) {
 	char *part = mnt.mnt_mountp;
-#else
-#if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV) || defined(AFS_LINUX22_ENV)
-#ifdef AFS_LINUX22_ENV
-    if ((mfd = setmntent("/proc/mounts", "r")) == NULL) {
-	if ((mfd = setmntent("/etc/mtab", "r")) == NULL) {
-	    return NULL;
-	}
-    }
-#else
+# else
+#  if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV)
     if ((mfd = setmntent(MOUNTED /*MNTTAB*/, "r")) == NULL) {
 	return NULL;
     }
-#endif
     while ((mntent = getmntent(mfd))) {
 	char *part = mntent->mnt_dir;
-#else
+#  else
     setfsent();
     while ((fsent = getfsent())) {
 	char *part = fsent->fs_file;
+#  endif
+# endif /* AFS_SGI_ENV */
 #endif
-#endif /* AFS_SGI_ENV */
-#endif
 	struct stat status;
 #ifdef	AFS_AIX_ENV
 	if (vmountp->vmt_flags & (MNT_READONLY | MNT_REMOVABLE | MNT_REMOTE))
 	    continue;		/* Ignore any "special" partitions */
 #else
-#ifdef	AFS_SUN5_ENV
+# ifdef	AFS_SUN5_ENV
 	/* Ignore non ufs or non read/write partitions */
 	if ((strcmp(mnt.mnt_fstype, "ufs") != 0)
 	    || (strncmp(mnt.mnt_mntopts, "ro,ignore", 9) == 0))
 	    continue;
-#else
-#if defined(AFS_LINUX22_ENV)
-	if (strcmp(mntent->mnt_type, "ext2"))
-	    continue;
-#else
-#if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV)
+# else
+#  if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV)
 	if (!hasmntopt(mntent, MNTOPT_RW))
 	    continue;
-#else
+#  else
 	if (strcmp(fsent->fs_type, "rw") != 0)
 	    continue;		/* Ignore non read/write partitions */
-#endif /* AFS_LINUX22_ENV */
-#endif /* AFS_SGI_ENV */
+#  endif /* AFS_SGI_ENV */
+# endif
 #endif
-#endif
 	/* Only keep track of "/vicepx" partitions since it can get hairy when NFS mounts are involved.. */
 	if (strncmp(part, VICE_PARTITION_PREFIX, VICE_PREFIX_SIZE)) {
 	    continue;		/* Non /vicepx; ignore */
@@ -163,7 +147,7 @@ vol_DevName(dev_t adev, char *wpath)
 	if (stat(part, &status) == -1) {
 	    continue;
 	}
-#if !defined(AFS_SGI_XFS_IOPS_ENV) && !defined(AFS_LINUX22_ENV) && !defined(AFS_DARWIN_ENV)
+#if !defined(AFS_SGI_XFS_IOPS_ENV)
 	if ((status.st_ino !=
 	     ROOTINO) /*|| ((status.st_mode & S_IFMT) != S_IFBLK) */ ) {
 	    continue;
@@ -173,16 +157,16 @@ vol_DevName(dev_t adev, char *wpath)
 #ifdef	AFS_AIX_ENV
 	    strcpy(pbuffer, vmt2dataptr(vmountp, VMT_OBJECT));
 #else
-#ifdef	AFS_SUN5_ENV
+# ifdef	AFS_SUN5_ENV
 	    strcpy(pbuffer, mnt.mnt_special);
-#else
-#if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV) || defined(AFS_LINUX22_ENV)
+# else
+#  if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV)
 	    strcpy(pbuffer, mntent->mnt_fsname);
-#else
+#  else
 	    strcpy(pbuffer, fsent->fs_spec);
+#  endif
+# endif /* AFS_SGI_ENV */
 #endif
-#endif /* AFS_SGI_ENV */
-#endif
 	    if (wpath) {
 		strcpy(pbuf, pbuffer);
 		ptr = (char *)strrchr(pbuf, OS_DIRSEPC);
@@ -203,13 +187,13 @@ vol_DevName(dev_t adev, char *wpath)
 #ifdef	AFS_SUN5_ENV
     (void)fclose(mntfile);
 #else
-#if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV) || defined(AFS_LINUX22_ENV)
+# if defined(AFS_SGI_ENV) || defined(AFS_SUN_ENV) || defined(AFS_HPUX_ENV)
     endmntent(mfd);
-#else
-#ifndef	AFS_AIX_ENV
+# else
+#  ifndef	AFS_AIX_ENV
     endfsent();
-#endif
-#endif
+#  endif
+# endif
 #endif /* AFS_SGI_ENV */
     return NULL;
 }
@@ -243,3 +227,5 @@ afs_rawname(char *devfile)
 
     return NULL;
 }
+
+#endif /* AFS_NAMEI_ENV */
