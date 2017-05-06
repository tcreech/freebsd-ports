--- src/afs/VNOPS/afs_vnop_strategy.c.orig	2016-12-08 04:01:51 UTC
+++ src/afs/VNOPS/afs_vnop_strategy.c
@@ -98,7 +98,7 @@ int afs_ustrategy(struct buf *abp)
 	tuio.afsio_fmode = 0;
 #endif
 	tuio.afsio_resid = abp->b_bcount;
-#if defined(AFS_NBSD40_ENV)
+#if defined(AFS_NBSD40_ENV) || defined(FBSD_STRUCT_BUF_NO_SAVEADDR)
 	tiovec[0].iov_base = abp->b_data;
 #elif defined(AFS_XBSD_ENV)
 	tiovec[0].iov_base = abp->b_saveaddr;
@@ -115,7 +115,7 @@ int afs_ustrategy(struct buf *abp)
 #endif
 	if (code == 0) {
 	    if (tuio.afsio_resid > 0)
-#if defined(AFS_NBSD40_ENV)
+#if defined(AFS_NBSD40_ENV) || defined(FBSD_STRUCT_BUF_NO_SAVEADDR)
 		memset((char *)abp->b_data + (uintptr_t)abp->b_bcount - tuio.afsio_resid, 0,
 		       tuio.afsio_resid);
 #elif defined(AFS_XBSD_ENV)
@@ -180,7 +180,7 @@ int afs_ustrategy(struct buf *abp)
 	len = MIN(len, tvc->f.m.Length - dbtob(abp->b_blkno));
 #endif
 	tuio.afsio_resid = len;
-#if defined(AFS_NBSD40_ENV)
+#if defined(AFS_NBSD40_ENV) || defined(FBSD_STRUCT_BUF_NO_SAVEADDR)
 	tiovec[0].iov_base = abp->b_data;
 #elif defined(AFS_XBSD_ENV)
 	tiovec[0].iov_base = abp->b_saveaddr;
