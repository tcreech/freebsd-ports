--- src/afs/DARWIN/osi_vfsops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/DARWIN/osi_vfsops.c
@@ -286,7 +286,7 @@ again:
     }
     if (tvp) {
 #ifndef AFS_DARWIN80_ENV /* KPI callers don't need a usecount reference */
-	osi_vnhold(tvp, 0);
+	osi_Assert(osi_vnhold(tvp) == 0);
 	AFS_GUNLOCK();
 	vn_lock(AFSTOV(tvp), LK_EXCLUSIVE | LK_RETRY, p);
 	AFS_GLOCK();
@@ -356,9 +356,6 @@ afs_statfs(struct mount *mp, STATFS_TYPE *abp, CTX_TYP
 #else
     abp->f_bsize = mp->vfs_bsize;
     abp->f_iosize = mp->vfs_bsize;
-#endif
-#if 0
-    abp->f_type = MOUNT_AFS;
 #endif
 
     abp->f_blocks = abp->f_bfree = abp->f_bavail = abp->f_files =
