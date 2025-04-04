--- src/afs/afs_pioctl.c.orig	2024-10-03 22:32:45 UTC
+++ src/afs/afs_pioctl.c
@@ -1250,8 +1250,12 @@ afs_syscall_pioctl(char *path, unsigned int com, caddr
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
