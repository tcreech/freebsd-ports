--- src/afs/HPUX/osi_vfsops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/HPUX/osi_vfsops.c
@@ -166,7 +166,7 @@ afs_statfs(struct vfs *afsp, struct k_statvfs *abp)
      */
     abp->f_blocks = abp->f_bfree = abp->f_bavail = abp->f_files =
 	abp->f_ffree = abp->f_favail = AFS_VFS_FAKEFREE;
-    abp->f_fsid = (AFS_VFSMAGIC << 16) || AFS_VFSFSID;
+    abp->f_fsid = (AFS_VFSMAGIC << 16) | AFS_VFSFSID;
 
     AFS_GUNLOCK();
     return 0;
