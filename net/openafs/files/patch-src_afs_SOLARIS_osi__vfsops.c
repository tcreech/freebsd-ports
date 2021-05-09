--- src/afs/SOLARIS/osi_vfsops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/SOLARIS/osi_vfsops.c
@@ -193,7 +193,7 @@ again:
 	}
     }
     if (tvp) {
-	AFS_FAST_HOLD(tvp);
+	osi_Assert(osi_vnhold(tvp) == 0);
 	mutex_enter(&AFSTOV(tvp)->v_lock);
 	AFSTOV(tvp)->v_flag |= VROOT;
 	mutex_exit(&AFSTOV(tvp)->v_lock);
@@ -229,7 +229,7 @@ afs_statvfs(struct vfs *afsp, struct statvfs64 *abp)
     abp->f_bsize = afsp->vfs_bsize;
     abp->f_blocks = abp->f_bfree = abp->f_bavail = abp->f_files =
 	abp->f_favail = abp->f_ffree = AFS_VFS_FAKEFREE;
-    abp->f_fsid = (AFS_VFSMAGIC << 16) || AFS_VFSFSID;
+    abp->f_fsid = (AFS_VFSMAGIC << 16) | AFS_VFSFSID;
 
     AFS_GUNLOCK();
     return 0;
