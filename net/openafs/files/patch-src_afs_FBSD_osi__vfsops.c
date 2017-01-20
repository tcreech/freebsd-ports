--- src/afs/FBSD/osi_vfsops.c.orig	2016-11-30 20:06:42 UTC
+++ src/afs/FBSD/osi_vfsops.c
@@ -142,6 +142,8 @@ afs_omount(struct mount *mp, char *path,
 #if __FreeBSD_version < 1000021
     mp->mnt_kern_flag |= MNTK_MPSAFE; /* solid steel */
 #endif
+    mp->mnt_kern_flag |= MNTK_NO_IOPF | MNTK_USES_BCACHE | MNTK_NOASYNC;
+    mp->mnt_kern_flag &= ~MNTK_SHARED_WRITES;
 #ifndef AFS_FBSD61_ENV
     MNT_ILOCK(mp);
 #endif
