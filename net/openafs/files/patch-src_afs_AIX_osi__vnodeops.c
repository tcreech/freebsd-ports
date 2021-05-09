--- src/afs/AIX/osi_vnodeops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/AIX/osi_vnodeops.c
@@ -1239,14 +1239,6 @@ afs_direct_rdwr(struct vnode *vp, 
 	goto fail;
     }
 
-    /* check for "file too big" error, which should really be done above us */
-#ifdef notdef
-    if (rw == UIO_WRITE && xfrSize + fileSize > get_ulimit()) {
-	code = EFBIG;
-	ReleaseReadLock(&vcp->lock);
-	goto fail;
-    }
-#endif
     ReleaseReadLock(&vcp->lock);
     if (rw == UIO_WRITE) {
 	ObtainWriteLock(&vcp->lock, 400);
