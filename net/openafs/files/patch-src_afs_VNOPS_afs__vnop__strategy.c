--- src/afs/VNOPS/afs_vnop_strategy.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/VNOPS/afs_vnop_strategy.c
@@ -208,10 +208,8 @@ int afs_ustrategy(struct buf *abp)
 
 #if defined(AFS_AIX32_ENV)
     crfree(credp);
-#elif defined(AFS_FBSD60_ENV)
-    (*abp->b_iodone)(abp);
 #elif defined(AFS_FBSD_ENV)
-    biodone(&abp->b_io);
+    (*abp->b_iodone)(abp);
 #elif defined(AFS_NBSD40_ENV)
     abp->b_resid = tuio.uio_resid;
     biodone(abp);
