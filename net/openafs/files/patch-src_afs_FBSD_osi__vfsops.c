--- src/afs/FBSD/osi_vfsops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/FBSD/osi_vfsops.c
@@ -15,99 +15,90 @@
 
 struct vcache *afs_globalVp = NULL;
 struct mount *afs_globalVFS = NULL;
+#if defined(AFS_FBSD_UMA_BUFS)
+uma_zone_t afs_pbuf_zone;
+#else
 int afs_pbuf_freecnt = -1;
+#endif
 
 extern int Afs_xsetgroups();
 extern int afs_xioctl();
 
-#if !defined(AFS_FBSD90_ENV) && !defined(AFS_FBSD82_ENV)
-static sy_call_t *old_handler;
-#else
-static struct sysent old_sysent;
-
-static struct sysent afs_sysent = {
-    5,			/* int sy_narg */
-    (sy_call_t *) afs3_syscall,	/* sy_call_t *sy_call */
-#ifdef AFS_FBSD60_ENV
-    AUE_NULL,		/* au_event_t sy_auevent */
-#ifdef AFS_FBSD70_ENV
-    NULL,		/* systrace_args_funt_t sy_systrace_args_func */
-    0,			/* u_int32_t sy_entry */
-    0,			/* u_int32_t sy_return */
-#ifdef AFS_FBSD90_ENV
-    0,			/* u_int32_t sy_flags */
-    0			/* u_int32_t sy_thrcnt */
-#endif
-#endif
-#endif /* FBSD60 */
+static struct syscall_helper_data afs_syscalls[] = {
+    {
+	.syscall_no = AFS_SYSCALL,
+	.new_sysent = {
+	    .sy_narg = 5,
+	    .sy_call = (sy_call_t *)afs3_syscall,
+	    .sy_auevent = AUE_NULL,
+	},
+    },
+    SYSCALL_INIT_LAST
 };
-#endif /* FBSD90 */
 
-int
+static int
 afs_init(struct vfsconf *vfc)
 {
     int code;
-    int offset = AFS_SYSCALL;
-#if defined(AFS_FBSD90_ENV) || defined(AFS_FBSD82_ENV)
-# if defined(FBSD_SYSCALL_REGISTER_FOUR_ARGS)
-    code = syscall_register(&offset, &afs_sysent, &old_sysent, 0);
-# else
-    code = syscall_register(&offset, &afs_sysent, &old_sysent);
-# endif
+#if defined(FBSD_SYSCALL_REGISTER_TAKES_FLAGS)
+    code = syscall_helper_register(afs_syscalls, 0);
+#else
+    code = syscall_helper_register(afs_syscalls);
+#endif
     if (code) {
 	printf("AFS_SYSCALL in use, error %i. aborting\n", code);
 	return code;
     }
-#else
-    if (sysent[AFS_SYSCALL].sy_call != (sy_call_t *)nosys
-        && sysent[AFS_SYSCALL].sy_call != (sy_call_t *)lkmnosys) {
-        printf("AFS_SYSCALL in use. aborting\n");
-        return EBUSY;
-    }
-#endif
     osi_Init();
+#if defined(AFS_FBSD_UMA_BUFS)
+    afs_pbuf_zone = pbuf_zsecond_create("afspbuf", nswbuf / 2);
+#else
     afs_pbuf_freecnt = nswbuf / 2 + 1;
-#if !defined(AFS_FBSD90_ENV) && !defined(AFS_FBSD82_ENV)
-    old_handler = sysent[AFS_SYSCALL].sy_call;
-    sysent[AFS_SYSCALL].sy_call = afs3_syscall;
-    sysent[AFS_SYSCALL].sy_narg = 5;
 #endif
     return 0;
 }
 
-int
+static int
 afs_uninit(struct vfsconf *vfc)
 {
-#if defined(AFS_FBSD90_ENV) || defined(AFS_FBSD82_ENV)
-    int offset = AFS_SYSCALL;
-#endif
-
     if (afs_globalVFS)
 	return EBUSY;
-#if defined(AFS_FBSD90_ENV) || defined(AFS_FBSD82_ENV)
-    syscall_deregister(&offset, &old_sysent);
-#else
-    sysent[AFS_SYSCALL].sy_narg = 0;
-    sysent[AFS_SYSCALL].sy_call = old_handler;
+
+#if defined(AFS_FBSD_UMA_BUFS)
+    uma_zdestroy(afs_pbuf_zone);
 #endif
-    return 0;
+
+    return syscall_helper_unregister(afs_syscalls);
 }
 
-int
-afs_start(struct mount *mp, int flags, struct thread *p)
+static int
+afs_statfs(struct mount *mp, struct statfs *abp)
 {
-    return (0);			/* nothing to do. ? */
+    AFS_GLOCK();
+    AFS_STATCNT(afs_statfs);
+
+    abp->f_bsize = mp->vfs_bsize;
+    abp->f_iosize = mp->vfs_bsize;
+
+    abp->f_blocks = abp->f_bfree = abp->f_bavail = abp->f_files =
+	abp->f_ffree = AFS_VFS_FAKEFREE;
+
+    abp->f_fsid.val[0] = mp->mnt_stat.f_fsid.val[0];
+    abp->f_fsid.val[1] = mp->mnt_stat.f_fsid.val[1];
+    if (abp != &mp->mnt_stat) {
+	abp->f_type = mp->mnt_vfc->vfc_typenum;
+	memcpy((caddr_t) & abp->f_mntonname[0],
+	       (caddr_t) mp->mnt_stat.f_mntonname, MNAMELEN);
+	memcpy((caddr_t) & abp->f_mntfromname[0],
+	       (caddr_t) mp->mnt_stat.f_mntfromname, MNAMELEN);
+    }
+
+    AFS_GUNLOCK();
+    return 0;
 }
 
-int
-#if defined(AFS_FBSD80_ENV)
+static int
 afs_omount(struct mount *mp, char *path, caddr_t data)
-#elif defined(AFS_FBSD53_ENV)
-afs_omount(struct mount *mp, char *path, caddr_t data, struct thread *p)
-#else
-afs_omount(struct mount *mp, char *path, caddr_t data, struct nameidata *ndp,
-	struct thread *p)
-#endif
 {
     /* ndp contains the mounted-from device.  Just ignore it.
      * we also don't care about our thread struct. */
@@ -132,19 +123,11 @@ afs_omount(struct mount *mp, char *path, caddr_t data,
      * more fields over time and there's not a good way to group the
      * code without duplication.
      */
-#ifdef AFS_FBSD62_ENV
     MNT_ILOCK(mp);
-#endif
     mp->mnt_flag &= ~MNT_LOCAL;
-#if defined(AFS_FBSD61_ENV) && !defined(AFS_FBSD62_ENV)
-    MNT_ILOCK(mp);
-#endif
 #if __FreeBSD_version < 1000021
     mp->mnt_kern_flag |= MNTK_MPSAFE; /* solid steel */
 #endif
-#ifndef AFS_FBSD61_ENV
-    MNT_ILOCK(mp);
-#endif
     /*
      * XXX mnt_stat "is considered stable as long as a ref is held".
      * We should check that we hold the only ref.
@@ -162,51 +145,29 @@ afs_omount(struct mount *mp, char *path, caddr_t data,
     strcpy(mp->mnt_stat.f_fstypename, "afs");
     MNT_IUNLOCK(mp);
     AFS_GUNLOCK();
-#ifdef AFS_FBSD80_ENV
     afs_statfs(mp, &mp->mnt_stat);
-#else
-    afs_statfs(mp, &mp->mnt_stat, p);
-#endif
 
     return 0;
 }
 
-#ifdef AFS_FBSD53_ENV
-int
-#ifdef AFS_FBSD80_ENV
+static int
 afs_mount(struct mount *mp)
-#else
-afs_mount(struct mount *mp, struct thread *td)
-#endif
 {
-#ifdef AFS_FBSD80_ENV
     return afs_omount(mp, NULL, NULL);
-#else
-    return afs_omount(mp, NULL, NULL, td);
-#endif
 }
-#endif
 
-#ifdef AFS_FBSD60_ENV
 static int
-#if (__FreeBSD_version >= 900503 && __FreeBSD_version < 1000000) || __FreeBSD_version >= 1000004
+#if __FreeBSD_version >= 1000004
 afs_cmount(struct mntarg *ma, void *data, uint64_t flags)
-#elif defined(AFS_FBSD80_ENV)
-afs_cmount(struct mntarg *ma, void *data, int flags)
 #else
-afs_cmount(struct mntarg *ma, void *data, int flags, struct thread *td)
+afs_cmount(struct mntarg *ma, void *data, int flags)
 #endif
 {
     return kernel_mount(ma, flags);
 }
-#endif
 
-int
-#ifdef AFS_FBSD80_ENV
+static int
 afs_unmount(struct mount *mp, int flags)
-#else
-afs_unmount(struct mount *mp, int flags, struct thread *p)
-#endif
 {
     int error = 0;
 
@@ -222,20 +183,13 @@ afs_unmount(struct mount *mp, int flags, struct thread
 	error = EBUSY;
     AFS_GUNLOCK();
 
-    /*
-     * Release any remaining vnodes on this mount point.
-     * The `1' means that we hold one extra reference on
-     * the root vnode (this is just a guess right now).
-     * This has to be done outside the global lock.
-     */
     if (!error) {
-#if defined(AFS_FBSD80_ENV)
-	error = vflush(mp, 1, (flags & MNT_FORCE) ? FORCECLOSE : 0, curthread);
-#elif defined(AFS_FBSD53_ENV)
-	error = vflush(mp, 1, (flags & MNT_FORCE) ? FORCECLOSE : 0, p);
-#else
-	error = vflush(mp, 1, (flags & MNT_FORCE) ? FORCECLOSE : 0);
-#endif
+	/*
+	 * Release any remaining vnodes on this mount point. The second
+	 * argument is how many refs we hold on the root vnode. Since we
+	 * released our reference to the root vnode up above, give 0.
+	 */
+	error = vflush(mp, 0, (flags & MNT_FORCE) ? FORCECLOSE : 0, curthread);
     }
     if (error)
 	goto out;
@@ -249,24 +203,14 @@ out:
     return error;
 }
 
-int
-#if defined(AFS_FBSD80_ENV)
+static int
 afs_root(struct mount *mp, int flags, struct vnode **vpp)
-#elif defined(AFS_FBSD60_ENV)
-afs_root(struct mount *mp, int flags, struct vnode **vpp, struct thread *td)
-#elif defined(AFS_FBSD53_ENV)
-afs_root(struct mount *mp, struct vnode **vpp, struct thread *td)
-#else
-afs_root(struct mount *mp, struct vnode **vpp)
-#endif
 {
     int error;
     struct vrequest treq;
     struct vcache *tvp = 0;
     struct vcache *gvp;
-#if !defined(AFS_FBSD53_ENV) || defined(AFS_FBSD80_ENV)
     struct thread *td = curthread;
-#endif
     struct ucred *cr = osi_curcred();
 
     AFS_GLOCK();
@@ -300,17 +244,20 @@ tryagain:
 
 	ASSERT_VI_UNLOCKED(vp, "afs_root");
 	AFS_GUNLOCK();
+#ifdef AFS_FBSD_VGET_NOTHREAD
+	error = vget(vp, LK_EXCLUSIVE | LK_RETRY);
+#else
 	error = vget(vp, LK_EXCLUSIVE | LK_RETRY, td);
+#endif /* AFS_FBSD_VGET_NOTHREAD */
 	AFS_GLOCK();
+	if (error != 0)
+	    goto tryagain;
 	/* we dropped the glock, so re-check everything it had serialized */
 	if (!afs_globalVp || !(afs_globalVp->f.states & CStatd) ||
 		tvp != afs_globalVp) {
 	    vput(vp);
-	    afs_PutVCache(tvp);
 	    goto tryagain;
 	}
-	if (error != 0)
-	    goto tryagain;
 	/*
 	 * I'm uncomfortable about this.  Shouldn't this happen at a
 	 * higher level, and shouldn't we busy the top-level directory
@@ -329,49 +276,12 @@ tryagain:
     return error;
 }
 
-int
-#ifdef AFS_FBSD80_ENV
-afs_statfs(struct mount *mp, struct statfs *abp)
-#else
-afs_statfs(struct mount *mp, struct statfs *abp, struct thread *p)
-#endif
-{
-    AFS_GLOCK();
-    AFS_STATCNT(afs_statfs);
-
-    abp->f_bsize = mp->vfs_bsize;
-    abp->f_iosize = mp->vfs_bsize;
-
-    abp->f_blocks = abp->f_bfree = abp->f_bavail = abp->f_files =
-	abp->f_ffree = AFS_VFS_FAKEFREE;
-
-    abp->f_fsid.val[0] = mp->mnt_stat.f_fsid.val[0];
-    abp->f_fsid.val[1] = mp->mnt_stat.f_fsid.val[1];
-    if (abp != &mp->mnt_stat) {
-	abp->f_type = mp->mnt_vfc->vfc_typenum;
-	memcpy((caddr_t) & abp->f_mntonname[0],
-	       (caddr_t) mp->mnt_stat.f_mntonname, MNAMELEN);
-	memcpy((caddr_t) & abp->f_mntfromname[0],
-	       (caddr_t) mp->mnt_stat.f_mntfromname, MNAMELEN);
-    }
-
-    AFS_GUNLOCK();
-    return 0;
-}
-
-int
-#if defined(AFS_FBSD80_ENV)
+static int
 afs_sync(struct mount *mp, int waitfor)
-#elif defined(AFS_FBSD60_ENV)
-afs_sync(struct mount *mp, int waitfor, struct thread *td)
-#else
-afs_sync(struct mount *mp, int waitfor, struct ucred *cred, struct thread *p)
-#endif
 {
     return 0;
 }
 
-#ifdef AFS_FBSD60_ENV
 struct vfsops afs_vfsops = {
 	.vfs_init =		afs_init,
 	.vfs_mount =		afs_mount,
@@ -383,25 +293,3 @@ struct vfsops afs_vfsops = {
 	.vfs_unmount =		afs_unmount,
 	.vfs_sysctl =		vfs_stdsysctl,
 };
-#else
-struct vfsops afs_vfsops = {
-#ifdef AFS_FBSD53_ENV
-    afs_mount,
-#endif
-    afs_omount,
-    afs_start,
-    afs_unmount,
-    afs_root,
-    vfs_stdquotactl,
-    afs_statfs,
-    afs_sync,
-    vfs_stdvget,
-    vfs_stdfhtovp,
-    vfs_stdcheckexp,
-    vfs_stdvptofh,
-    afs_init,
-    afs_uninit,
-    vfs_stdextattrctl,
-    vfs_stdsysctl,
-};
-#endif
