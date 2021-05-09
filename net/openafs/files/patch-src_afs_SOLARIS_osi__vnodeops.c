--- src/afs/SOLARIS/osi_vnodeops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/SOLARIS/osi_vnodeops.c
@@ -695,11 +695,7 @@ afs_nfsrdwr(struct vcache *avc, struct uio *auio, enum
     afs_MaybeWakeupTruncateDaemon();
     while ((arw == UIO_WRITE)
 	   && (afs_blocksUsed > PERCENT(CM_WAITFORDRAINPCT, afs_cacheBlocks))) {
-	if (afs_blocksUsed - afs_blocksDiscarded >
-	    PERCENT(CM_WAITFORDRAINPCT, afs_cacheBlocks)) {
-	    afs_WaitForCacheDrain = 1;
-	    afs_osi_Sleep(&afs_WaitForCacheDrain);
-	}
+	afs_MaybeWaitForCacheDrain();
 	afs_MaybeFreeDiscardedDCache();
 	afs_MaybeWakeupTruncateDaemon();
     }
