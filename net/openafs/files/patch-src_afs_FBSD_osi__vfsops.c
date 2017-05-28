--- src/afs/FBSD/osi_vfsops.c.orig	2016-12-08 04:01:51 UTC
+++ src/afs/FBSD/osi_vfsops.c
@@ -297,29 +297,28 @@ tryagain:
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
