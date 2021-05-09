--- src/afs/FBSD/osi_file.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/FBSD/osi_file.c
@@ -17,9 +17,6 @@
 
 
 int afs_osicred_initialized = 0;
-#ifndef AFS_FBSD80_ENV	/* cr_groups is now malloc()'d */
-afs_ucred_t afs_osi_cred;
-#endif
 extern struct osi_dev cacheDev;
 extern struct mount *afs_cacheVfsp;
 
@@ -45,11 +42,11 @@ osi_UFSOpen(afs_dcache_id_t *ainode)
 	osi_FreeSmallSpace(afile);
 	osi_Panic("UFSOpen: igetinode failed");
     }
-#if defined(AFS_FBSD80_ENV)
-    VOP_UNLOCK(vp, 0);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(vp);
 #else
-    VOP_UNLOCK(vp, 0, curthread);
-#endif
+    VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     afile->vnode = vp;
     afile->size = VTOI(vp)->i_size;
     afile->offset = 0;
@@ -64,15 +61,13 @@ afs_osi_Stat(struct osi_file *afile, struct osi_stat *
     struct vattr tvattr;
     AFS_STATCNT(osi_Stat);
     AFS_GUNLOCK();
-#if defined(AFS_FBSD80_ENV)
     vn_lock(afile->vnode, LK_EXCLUSIVE | LK_RETRY);
     code = VOP_GETATTR(afile->vnode, &tvattr, afs_osi_credp);
-    VOP_UNLOCK(afile->vnode, 0);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(afile->vnode);
 #else
-    vn_lock(afile->vnode, LK_EXCLUSIVE | LK_RETRY, curthread);
-    code = VOP_GETATTR(afile->vnode, &tvattr, afs_osi_credp, curthread);
-    VOP_UNLOCK(afile->vnode, LK_EXCLUSIVE, curthread);
-#endif
+    VOP_UNLOCK(afile->vnode, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     AFS_GLOCK();
     if (code == 0) {
 	astat->size = tvattr.va_size;
@@ -111,30 +106,21 @@ osi_UFSTruncate(struct osi_file *afile, afs_int32 asiz
     glocked = ISAFS_GLOCK();
     if (glocked)
       AFS_GUNLOCK();
-#if defined(AFS_FBSD80_ENV)
     vn_lock(vp, LK_EXCLUSIVE | LK_RETRY);
     code = VOP_GETATTR(afile->vnode, &tvattr, afs_osi_credp);
-#else
-    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY, curthread);
-    code = VOP_GETATTR(afile->vnode, &tvattr, afs_osi_credp, curthread);
-#endif
     if (code != 0 || tvattr.va_size <= asize)
 	goto out;
 
     VATTR_NULL(&tvattr);
     tvattr.va_size = asize;
-#if defined(AFS_FBSD80_ENV)
     code = VOP_SETATTR(vp, &tvattr, afs_osi_credp);
-#else
-    code = VOP_SETATTR(vp, &tvattr, afs_osi_credp, curthread);
-#endif
 
 out:
-#if defined(AFS_FBSD80_ENV)
-    VOP_UNLOCK(vp, 0);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(vp);
 #else
-    VOP_UNLOCK(vp, LK_EXCLUSIVE, curthread);
-#endif
+    VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
     if (glocked)
       AFS_GLOCK();
     return code;
@@ -153,7 +139,7 @@ int
 afs_osi_Read(struct osi_file *afile, int offset, void *aptr,
 	     afs_int32 asize)
 {
-#if (__FreeBSD_version >= 900505 && __FreeBSD_Version < 1000000) ||__FreeBSD_version >= 1000009
+#if __FreeBSD_version >= 1000009
     ssize_t resid;
 #else
     int resid;
@@ -198,7 +184,7 @@ int
 afs_osi_Write(struct osi_file *afile, afs_int32 offset, void *aptr,
 	      afs_int32 asize)
 {
-#if (__FreeBSD_version >= 900505 && __FreeBSD_Version < 1000000) ||__FreeBSD_version >= 1000009
+#if __FreeBSD_version >= 1000009
     ssize_t resid;
 #else
     int resid;
