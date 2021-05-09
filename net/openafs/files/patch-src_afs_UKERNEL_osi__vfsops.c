--- src/afs/UKERNEL/osi_vfsops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/UKERNEL/osi_vfsops.c
@@ -129,7 +129,7 @@ afs_statvfs(struct vfs *afsp, struct statvfs *abp)
     abp->f_fsid.val[0] = AFS_VFSMAGIC;
     abp->f_fsid.val[1] = AFS_VFSFSID;
 #else
-    abp->f_fsid = (AFS_VFSMAGIC << 16) || AFS_VFSFSID;
+    abp->f_fsid = (AFS_VFSMAGIC << 16) | AFS_VFSFSID;
 #endif
 
     return 0;
