--- src/afs/FBSD/osi_vfsops.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/FBSD/osi_vfsops.c
@@ -15,58 +15,45 @@
 
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
-    AUE_NULL,		/* au_event_t sy_auevent */
-    NULL,		/* systrace_args_funt_t sy_systrace_args_func */
-    0,			/* u_int32_t sy_entry */
-    0,			/* u_int32_t sy_return */
-# ifdef AFS_FBSD90_ENV
-    0,			/* u_int32_t sy_flags */
-    0			/* u_int32_t sy_thrcnt */
-# endif
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
 
 static int
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
@@ -74,19 +61,14 @@ afs_init(struct vfsconf *vfc)
 static int
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
 
 static int
@@ -175,7 +157,7 @@ afs_mount(struct mount *mp)
 }
 
 static int
-#if (__FreeBSD_version >= 900503 && __FreeBSD_version < 1000000) || __FreeBSD_version >= 1000004
+#if __FreeBSD_version >= 1000004
 afs_cmount(struct mntarg *ma, void *data, uint64_t flags)
 #else
 afs_cmount(struct mntarg *ma, void *data, int flags)
@@ -262,17 +244,20 @@ tryagain:
 
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
