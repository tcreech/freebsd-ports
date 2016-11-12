--- src/afs/VNOPS/afs_vnop_write.c.orig	2016-08-03 14:45:27 UTC
+++ src/afs/VNOPS/afs_vnop_write.c
@@ -281,7 +281,7 @@ afs_MemWrite(struct vcache *avc, struct 
 #endif
 	ReleaseWriteLock(&tdc->lock);
 	afs_PutDCache(tdc);
-#if !defined(AFS_VM_RDWR_ENV)
+#if !defined(AFS_VM_RDWR_ENV) || defined(AFS_XBSD_ENV)
 	/*
 	 * If write is implemented via VM, afs_DoPartialWrite() is called from
 	 * the high-level write op.
@@ -590,7 +590,7 @@ afs_UFSWrite(struct vcache *avc, struct 
 	osi_UFSClose(tfile);
 	ReleaseWriteLock(&tdc->lock);
 	afs_PutDCache(tdc);
-#if !defined(AFS_VM_RDWR_ENV)
+#if !defined(AFS_VM_RDWR_ENV) || defined(AFS_XBSD_ENV)
 	/*
 	 * If write is implemented via VM, afs_DoPartialWrite() is called from
 	 * the high-level write op.
