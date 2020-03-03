--- src/afs/FBSD/osi_vfsops.c.orig	2020-02-21 20:17:12 UTC
+++ src/afs/FBSD/osi_vfsops.c
@@ -38,11 +38,19 @@ afs_init(struct vfsconf *vfc)
 {
     int code;
     int offset = AFS_SYSCALL;
+#if defined(AFS_FBSD120_ENV)
+    struct syscall_helper_data afs_syscalls[] = {
+        { .new_sysent = afs_sysent, .syscall_no = AFS_SYSCALL },
+        SYSCALL_INIT_LAST
+    };
+    syscall_helper_register(afs_syscalls, 0);
+#else
 #if defined(FBSD_SYSCALL_REGISTER_FOUR_ARGS)
     code = syscall_register(&offset, &afs_sysent, &old_sysent, 0);
 #else
     code = syscall_register(&offset, &afs_sysent, &old_sysent);
 #endif
+#endif /* AFS_FBSD120_ENV */
     if (code) {
 	printf("AFS_SYSCALL in use, error %i. aborting\n", code);
 	return code;
@@ -59,7 +67,15 @@ afs_uninit(struct vfsconf *vfc)
 
     if (afs_globalVFS)
 	return EBUSY;
+#if defined(AFS_FBSD120_ENV)
+    struct syscall_helper_data afs_syscalls[] = {
+        { .old_sysent = old_sysent, .syscall_no = AFS_SYSCALL },
+        SYSCALL_INIT_LAST
+    };
+    syscall_helper_unregister(afs_syscalls);
+#else
     syscall_deregister(&offset, &old_sysent);
+#endif /* AFS_FBSD120_ENV */
     return 0;
 }
 
@@ -233,29 +249,28 @@ tryagain:
     }
     if (tvp) {
 	struct vnode *vp = AFSTOV(tvp);
+	afs_globalVFS = mp;
+	*vpp = vp;
 
-	ASSERT_VI_UNLOCKED(vp, "afs_root");
+	/* Drop the lock for the the vget. Otherwise, we have a lock order
+	 * reversal. */
 	AFS_GUNLOCK();
+	ASSERT_VI_UNLOCKED(vp, "afs_root");
 	error = vget(vp, LK_EXCLUSIVE | LK_RETRY, td);
 	AFS_GLOCK();
-	/* we dropped the glock, so re-check everything it had serialized */
-	if (!afs_globalVp || !(afs_globalVp->f.states & CStatd) ||
-		tvp != afs_globalVp) {
-	    vput(vp);
-	    afs_PutVCache(tvp);
-	    goto tryagain;
-	}
-	if (error != 0)
-	    goto tryagain;
-	/*
-	 * I'm uncomfortable about this.  Shouldn't this happen at a
-	 * higher level, and shouldn't we busy the top-level directory
-	 * to prevent recycling?
-	 */
-	vp->v_vflag |= VV_ROOT;
 
-	afs_globalVFS = mp;
-	*vpp = vp;
+	if (error != 0) {
+	    /* If we had an error, don't expose the vp. */
+	    *vpp = NULL;
+	    error = EIO;
+	} else {
+	    /*
+	     * I'm uncomfortable about this.  Shouldn't this happen at a
+	     * higher level, and shouldn't we busy the top-level directory to
+	     * prevent recycling?
+	     */
+	    vp->v_vflag |= VV_ROOT;
+        }
     }
 
     afs_Trace2(afs_iclSetp, CM_TRACE_VFSROOT, ICL_TYPE_POINTER, tvp ? AFSTOV(tvp) : NULL,
