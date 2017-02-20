--- src/afs/FBSD/osi_vfsops.c.orig	2016-11-30 20:06:42 UTC
+++ src/afs/FBSD/osi_vfsops.c
@@ -297,20 +297,6 @@ tryagain:
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
@@ -320,11 +306,22 @@ tryagain:
 
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
