--- src/afs/afs_pioctl.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/afs_pioctl.c
@@ -1255,8 +1255,12 @@ afs_syscall_pioctl(char *path, unsigned int com, caddr
 	AFS_GLOCK();
 #else
 #if defined(AFS_FBSD_ENV)
-    if (VOP_ISLOCKED(vp))
+    if (VOP_ISLOCKED(vp) == LK_EXCLUSIVE)
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(vp);
+#else
 	VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
 #endif /* AFS_FBSD_ENV */
 	AFS_RELE(vp);		/* put vnode back */
 #endif
