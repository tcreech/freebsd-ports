--- src/afs/FBSD/osi_file.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/FBSD/osi_file.c
@@ -42,7 +42,11 @@ osi_UFSOpen(afs_dcache_id_t *ainode)
 	osi_FreeSmallSpace(afile);
 	osi_Panic("UFSOpen: igetinode failed");
     }
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(vp);
+#else
     VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     afile->vnode = vp;
     afile->size = VTOI(vp)->i_size;
     afile->offset = 0;
@@ -59,7 +63,11 @@ afs_osi_Stat(struct osi_file *afile, struct osi_stat *
     AFS_GUNLOCK();
     vn_lock(afile->vnode, LK_EXCLUSIVE | LK_RETRY);
     code = VOP_GETATTR(afile->vnode, &tvattr, afs_osi_credp);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(afile->vnode);
+#else
     VOP_UNLOCK(afile->vnode, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     AFS_GLOCK();
     if (code == 0) {
 	astat->size = tvattr.va_size;
@@ -108,7 +116,11 @@ osi_UFSTruncate(struct osi_file *afile, afs_int32 asiz
     code = VOP_SETATTR(vp, &tvattr, afs_osi_credp);
 
 out:
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(vp);
+#else
     VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     if (glocked)
       AFS_GLOCK();
     return code;
