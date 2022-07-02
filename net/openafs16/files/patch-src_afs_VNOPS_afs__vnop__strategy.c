--- src/afs/VNOPS/afs_vnop_strategy.c.orig	2016-08-03 14:45:27 UTC
+++ src/afs/VNOPS/afs_vnop_strategy.c
@@ -99,7 +99,7 @@ int afs_ustrategy(struct buf *abp)
 #endif
 	tuio.afsio_resid = abp->b_bcount;
 #if defined(AFS_XBSD_ENV)
-	tiovec[0].iov_base = abp->b_saveaddr;
+	tiovec[0].iov_base = abp->b_data;
 #else
 	tiovec[0].iov_base = abp->b_un.b_addr;
 #endif /* AFS_XBSD_ENV */
@@ -114,7 +114,7 @@ int afs_ustrategy(struct buf *abp)
 	if (code == 0) {
 	    if (tuio.afsio_resid > 0)
 #if defined(AFS_XBSD_ENV)
-		memset(abp->b_saveaddr + abp->b_bcount - tuio.afsio_resid, 0,
+		memset(abp->b_data + abp->b_bcount - tuio.afsio_resid, 0,
 		       tuio.afsio_resid);
 #else
 		memset(abp->b_un.b_addr + abp->b_bcount - tuio.afsio_resid, 0,
@@ -176,7 +176,7 @@ int afs_ustrategy(struct buf *abp)
 #endif
 	tuio.afsio_resid = len;
 #if defined(AFS_XBSD_ENV)
-	tiovec[0].iov_base = abp->b_saveaddr;
+	tiovec[0].iov_base = abp->b_data;
 #else
 	tiovec[0].iov_base = abp->b_un.b_addr;
 #endif /* AFS_XBSD_ENV */
