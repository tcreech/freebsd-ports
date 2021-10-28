--- src/afs/afs_pioctl.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/afs_pioctl.c
@@ -828,7 +828,7 @@ afs_xioctl(afs_proc_t *p, const struct ioctl_args *uap
 #if defined(AFS_NBSD50_ENV)
     if ((fd = fd_getfile(SCARG(uap, fd))) == NULL)
 	return (EBADF);
-#elif defined(AFS_FBSD100_ENV)
+#elif defined(AFS_FBSD_ENV)
     if ((uap->fd >= fdp->fd_nfiles)
 	|| ((fd = fdp->fd_ofiles[uap->fd].fde_file) == NULL))
 	return EBADF;
@@ -887,11 +887,7 @@ afs_xioctl(afs_proc_t *p, const struct ioctl_args *uap
 
     if (!ioctlDone) {
 # if defined(AFS_FBSD_ENV)
-#  if (__FreeBSD_version >= 900044)
 	return sys_ioctl(td, uap);
-#  else
-	return ioctl(td, uap);
-#  endif
 # elif defined(AFS_OBSD_ENV)
 	code = sys_ioctl(p, uap, retval);
 # elif defined(AFS_NBSD_ENV)
@@ -1259,8 +1255,12 @@ afs_syscall_pioctl(char *path, unsigned int com, caddr
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
@@ -3523,7 +3523,9 @@ FlushVolumeData(struct VenusFid *afid, afs_ucred_t * a
 		    continue;
 		}
 #else
-		AFS_FAST_HOLD(tvc);
+		if (osi_vnhold(tvc) != 0) {
+		    continue;
+		}
 #endif
 		ReleaseReadLock(&afs_xvcache);
 		ObtainWriteLock(&tvc->lock, 232);
