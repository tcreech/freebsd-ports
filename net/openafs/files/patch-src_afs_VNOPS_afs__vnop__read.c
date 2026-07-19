--- src/afs/VNOPS/afs_vnop_read.c.orig	2022-12-15 20:10:23 UTC
+++ src/afs/VNOPS/afs_vnop_read.c
@@ -563,7 +563,11 @@ afs_UFSReadUIO(afs_dcache_id_t *cacheId, struct uio *t
     AFS_GUNLOCK();
     VOP_LOCK(tfile->vnode, LK_EXCLUSIVE);
     code = VOP_READ(tfile->vnode, tuiop, 0, afs_osi_credp);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(tfile->vnode);
+#else
     VOP_UNLOCK(tfile->vnode, 0);
+#endif
     AFS_GLOCK();
 #elif defined(AFS_NBSD_ENV)
     tuiop->uio_rw = UIO_READ;
