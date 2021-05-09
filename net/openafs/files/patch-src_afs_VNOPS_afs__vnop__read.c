--- src/afs/VNOPS/afs_vnop_read.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/VNOPS/afs_vnop_read.c
@@ -562,17 +562,15 @@ afs_UFSReadUIO(afs_dcache_id_t *cacheId, struct uio *t
     code = VOP_READ(tfile->vnode, tuiop, 0, afs_osi_credp);
     VOP_UNLOCK(tfile->vnode, 0, current_proc());
     AFS_GLOCK();
-#elif defined(AFS_FBSD80_ENV)
+#elif defined(AFS_FBSD_ENV)
     AFS_GUNLOCK();
     VOP_LOCK(tfile->vnode, LK_EXCLUSIVE);
     code = VOP_READ(tfile->vnode, tuiop, 0, afs_osi_credp);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(tfile->vnode);
+#else
     VOP_UNLOCK(tfile->vnode, 0);
-    AFS_GLOCK();
-#elif defined(AFS_FBSD_ENV)
-    AFS_GUNLOCK();
-    VOP_LOCK(tfile->vnode, LK_EXCLUSIVE, curthread);
-    code = VOP_READ(tfile->vnode, tuiop, 0, afs_osi_credp);
-    VOP_UNLOCK(tfile->vnode, 0, curthread);
+#endif
     AFS_GLOCK();
 #elif defined(AFS_NBSD_ENV)
     tuiop->uio_rw = UIO_READ;
