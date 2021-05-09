--- src/afs/afs_pioctl.c.orig	2021-01-14 21:08:41 UTC
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
@@ -1258,10 +1254,14 @@ afs_syscall_pioctl(char *path, unsigned int com, caddr
 	dput(dp);
 	AFS_GLOCK();
 #else
-#if defined(AFS_FBSD80_ENV)
-    if (VOP_ISLOCKED(vp))
+#if defined(AFS_FBSD_ENV)
+    if (VOP_ISLOCKED(vp) == LK_EXCLUSIVE)
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(vp);
+#else
 	VOP_UNLOCK(vp, 0);
-#endif /* AFS_FBSD80_ENV */
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
+#endif /* AFS_FBSD_ENV */
 	AFS_RELE(vp);		/* put vnode back */
 #endif
     }
@@ -1832,19 +1832,40 @@ _settok_tokenCell(char *cellName, int *cellNum, int *p
 }
 
 
+#if defined(AFS_LINUX26_ENV)
 static_inline int
-_settok_setParentPag(afs_ucred_t **cred) {
+_settok_setParentPag(afs_ucred_t **cred)
+{
     afs_uint32 pag;
-#if defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
+    int code;
+    afs_ucred_t *old_cred = *cred;
+    code = setpag(cred, -1, &pag, 1);
+    if (code == 0) {
+	/* setpag() may have changed our credentials */
+	*cred = crref();
+	crfree(old_cred);
+    }
+    return code;
+}
+#elif defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
+static_inline int
+_settok_setParentPag(afs_ucred_t **cred)
+{
+    afs_uint32 pag;
     char procname[256];
     osi_procname(procname, 256);
     afs_warnuser("Process %d (%s) tried to change pags in PSetTokens\n",
 	         MyPidxx2Pid(MyPidxx), procname);
     return setpag(osi_curproc(), cred, -1, &pag, 1);
+}
 #else
+static_inline int
+_settok_setParentPag(afs_ucred_t **cred)
+{
+    afs_uint32 pag;
     return setpag(cred, -1, &pag, 1);
-#endif
 }
+#endif
 
 /*!
  * VIOCSETTOK (3) - Set authentication tokens
@@ -3502,7 +3523,9 @@ FlushVolumeData(struct VenusFid *afid, afs_ucred_t * a
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
@@ -4621,7 +4644,7 @@ HandleClientContext(struct afs_ioctl *ablob, int *com,
 #ifdef AFS_AIX_ENV
     newcred->cr_ngrps = 2;
 #elif !defined(AFS_LINUX26_ENV) && !defined(AFS_SUN510_ENV)
-# if defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_LINUX22_ENV) || defined(AFS_FBSD80_ENV)
+# if defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_LINUX22_ENV) || defined(AFS_FBSD_ENV)
     newcred->cr_ngroups = 2;
 # else
     for (i = 2; i < NGROUPS; i++)
@@ -4766,10 +4789,6 @@ DECL_PIOCTL(PSetCPrefs)
 
     if (ainSize < sizeof(struct setspref))
 	return EINVAL;
-#if 0				/* num_servers is unsigned */
-    if (sin->num_servers < 0)
-	return EINVAL;
-#endif
     if (sin->num_servers > AFS_MAX_INTERFACE_ADDR)
 	return ENOMEM;
 
@@ -5369,15 +5388,7 @@ DECL_PIOCTL(PSetTokens2)
     }
 
     if (tokenSet.flags & AFSTOKEN_EX_SETPAG) {
-#if defined(AFS_LINUX26_ENV)
-	afs_ucred_t *old_cred = *acred;
-#endif
 	if (_settok_setParentPag(acred) == 0) {
-#if defined(AFS_LINUX26_ENV)
-	    /* setpag() may have changed our credentials */
-	    *acred = crref();
-	    crfree(old_cred);
-#endif
 	    code = afs_CreateReq(&treq, *acred);
 	    if (code) {
 		xdr_free((xdrproc_t) xdr_ktc_setTokenData, &tokenSet);
