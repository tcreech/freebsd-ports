--- src/afs/FBSD/osi_vfsops.c.orig	2018-09-11 15:54:34 UTC
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
 # if defined(AFS_FBSD110_ENV)
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
@@ -220,13 +232,11 @@ afs_unmount(struct mount *mp, int flags, struct thread
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
 #if defined(AFS_FBSD80_ENV)
@@ -237,9 +247,10 @@ afs_unmount(struct mount *mp, int flags, struct thread
 	error = vflush(mp, 1, (flags & MNT_FORCE) ? FORCECLOSE : 0);
 #endif
     }
-    if (error)
+    if (error) {
+        AFS_GUNLOCK();
 	goto out;
-    AFS_GLOCK();
+    }
     AFS_STATCNT(afs_unmount);
     afs_globalVFS = 0;
     afs_shutdown();
@@ -297,20 +308,6 @@ tryagain:
     }
     if (tvp) {
 	struct vnode *vp = AFSTOV(tvp);
-
-	ASSERT_VI_UNLOCKED(vp, "afs_root");
-	AFS_GUNLOCK();
-	error = vget(vp, LK_EXCLUSIVE | LK_RETRY, td);
-	AFS_GLOCK();
-	/* we dropped the glock, so re-check everything it had serialized */
-	if (!afs_globalVp || !(afs_globalVp->f.states & CStatd) ||
-		tvp != afs_globalVp) {
-	    vput(vp);
-	    afs_PutVCache(tvp);
-	    goto tryagain;
-	}
-	if (error != 0)
-	    goto tryagain;
 	/*
 	 * I'm uncomfortable about this.  Shouldn't this happen at a
 	 * higher level, and shouldn't we busy the top-level directory
@@ -320,11 +317,22 @@ tryagain:
 
 	afs_globalVFS = mp;
 	*vpp = vp;
+	// Drop the lock; we'll only do the vget afterward.
+	AFS_GUNLOCK();
+
+	ASSERT_VI_UNLOCKED(vp, "afs_root");
+	error = vget(vp, LK_EXCLUSIVE | LK_RETRY, td);
+        // If we had an error, don't expose the vp.
+	if (error != 0)
+	    *vpp = NULL;
+    } else {
+        // How often do we get here?
+        afs_warn("afs_root has NULL tvp?\n");
+        AFS_GUNLOCK();
     }
 
     afs_Trace2(afs_iclSetp, CM_TRACE_VFSROOT, ICL_TYPE_POINTER, tvp ? AFSTOV(tvp) : NULL,
 	       ICL_TYPE_INT32, error);
-    AFS_GUNLOCK();
     crfree(cr);
     return error;
 }
