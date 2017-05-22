--- src/afs/FBSD/osi_vfsops.c.orig	2016-12-14 18:28:29 UTC
+++ src/afs/FBSD/osi_vfsops.c
@@ -220,13 +220,11 @@ afs_unmount(struct mount *mp, int flags,
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
@@ -237,9 +235,10 @@ afs_unmount(struct mount *mp, int flags,
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
@@ -263,7 +262,6 @@ afs_root(struct mount *mp, struct vnode 
     int error;
     struct vrequest treq;
     struct vcache *tvp = 0;
-    struct vcache *gvp;
 #if !defined(AFS_FBSD53_ENV) || defined(AFS_FBSD80_ENV)
     struct thread *td = curthread;
 #endif
@@ -272,54 +270,44 @@ afs_root(struct mount *mp, struct vnode 
     AFS_GLOCK();
     AFS_STATCNT(afs_root);
     crhold(cr);
-tryagain:
+
     if (afs_globalVp && (afs_globalVp->f.states & CStatd)) {
 	tvp = afs_globalVp;
 	error = 0;
     } else {
 	if (!(error = afs_InitReq(&treq, cr)) && !(error = afs_CheckInit())) {
 	    tvp = afs_GetVCache(&afs_rootFid, &treq, NULL, NULL);
-	    /* we really want this to stay around */
-	    if (tvp) {
-		gvp = afs_globalVp;
+	    if (tvp){
+                /* There should be no race here.*/
+                KASSERT(!afs_globalVp || afs_globalVp == tvp,
+                        ("afs_root: afs_globalVP initialized twice"));
 		afs_globalVp = tvp;
-		if (gvp) {
-		    afs_PutVCache(gvp);
-		    if (tvp != afs_globalVp) {
-			/* someone raced us and won */
-			afs_PutVCache(tvp);
-			goto tryagain;
-		    }
-		}
-	    } else
+            }
+	    else
 		error = ENOENT;
 	}
     }
     if (tvp) {
 	struct vnode *vp = AFSTOV(tvp);
 
-	ASSERT_VI_UNLOCKED(vp, "afs_root");
+	afs_globalVFS = mp;
+
 	AFS_GUNLOCK();
+	ASSERT_VI_UNLOCKED(vp, "afs_root");
 	error = vget(vp, LK_EXCLUSIVE | LK_RETRY, td);
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
-	/*
-	 * I'm uncomfortable about this.  Shouldn't this happen at a
-	 * higher level, and shouldn't we busy the top-level directory
-	 * to prevent recycling?
-	 */
-	vp->v_vflag |= VV_ROOT;
+        AFS_GLOCK();
 
-	afs_globalVFS = mp;
-	*vpp = vp;
+	if (error != 0) {
+            error = EIO;
+        } else {
+            /*
+             * I'm uncomfortable about this.  Shouldn't this happen at a
+             * higher level, and shouldn't we busy the top-level directory
+             * to prevent recycling?
+             */
+            vp->v_vflag |= VV_ROOT;
+            *vpp = vp;
+        }
     }
 
     afs_Trace2(afs_iclSetp, CM_TRACE_VFSROOT, ICL_TYPE_POINTER, tvp ? AFSTOV(tvp) : NULL,
