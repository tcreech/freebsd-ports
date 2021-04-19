--- src/afs/FBSD/osi_vfsops.c.orig	2018-04-06 01:21:12 UTC
+++ src/afs/FBSD/osi_vfsops.c
@@ -48,7 +48,13 @@ afs_init(struct vfsconf *vfc)
 {
     int code;
     int offset = AFS_SYSCALL;
-#if defined(AFS_FBSD90_ENV) || defined(AFS_FBSD82_ENV)
+#if defined(AFS_FBSD120_ENV)
+    struct syscall_helper_data afs_syscalls[] = {
+        { .new_sysent = afs_sysent, .syscall_no = AFS_SYSCALL },
+        SYSCALL_INIT_LAST
+    };
+    syscall_helper_register(afs_syscalls, 0);
+#elif defined(AFS_FBSD90_ENV) || defined(AFS_FBSD82_ENV)
 # if defined(FBSD_SYSCALL_REGISTER_FOUR_ARGS)
     code = syscall_register(&offset, &afs_sysent, &old_sysent, 0);
 # else
@@ -84,7 +90,13 @@ afs_uninit(struct vfsconf *vfc)
 
     if (afs_globalVFS)
 	return EBUSY;
-#if defined(AFS_FBSD90_ENV) || defined(AFS_FBSD82_ENV)
+#if defined(AFS_FBSD120_ENV)
+    struct syscall_helper_data afs_syscalls[] = {
+        { .old_sysent = old_sysent, .syscall_no = AFS_SYSCALL },
+        SYSCALL_INIT_LAST
+    };
+    syscall_helper_unregister(afs_syscalls);
+#elif defined(AFS_FBSD90_ENV) || defined(AFS_FBSD82_ENV)
     syscall_deregister(&offset, &old_sysent);
 #else
     sysent[AFS_SYSCALL].sy_narg = 0;
@@ -220,15 +232,14 @@ afs_unmount(struct mount *mp, int flags, struct thread
     }
     if (afs_globalVp)
 	error = EBUSY;
-    AFS_GUNLOCK();
 
     /*
      * Release any remaining vnodes on this mount point.
      * The `1' means that we hold one extra reference on
      * the root vnode (this is just a guess right now).
-     * This has to be done outside the global lock.
      */
     if (!error) {
+	AFS_GUNLOCK();
 #if defined(AFS_FBSD80_ENV)
 	error = vflush(mp, 1, (flags & MNT_FORCE) ? FORCECLOSE : 0, curthread);
 #elif defined(AFS_FBSD53_ENV)
@@ -236,10 +247,12 @@ afs_unmount(struct mount *mp, int flags, struct thread
 #else
 	error = vflush(mp, 1, (flags & MNT_FORCE) ? FORCECLOSE : 0);
 #endif
+	AFS_GLOCK();
     }
-    if (error)
+    if (error) {
+	AFS_GUNLOCK();
 	goto out;
-    AFS_GLOCK();
+    }
     AFS_STATCNT(afs_unmount);
     afs_globalVFS = 0;
     afs_shutdown();
@@ -297,29 +310,28 @@ tryagain:
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
