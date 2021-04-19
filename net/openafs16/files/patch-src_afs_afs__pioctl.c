--- src/afs/afs_pioctl.c.orig	2017-01-20 04:12:52 UTC
+++ src/afs/afs_pioctl.c
@@ -1239,7 +1239,7 @@ afs_syscall_pioctl(char *path, unsigned 
 	AFS_GLOCK();
 #else
 #if defined(AFS_FBSD80_ENV)
-    if (VOP_ISLOCKED(vp))
+    if (VOP_ISLOCKED(vp) == LK_EXCLUSIVE)
 	VOP_UNLOCK(vp, 0);
 #endif /* AFS_FBSD80_ENV */
 	AFS_RELE(vp);		/* put vnode back */
