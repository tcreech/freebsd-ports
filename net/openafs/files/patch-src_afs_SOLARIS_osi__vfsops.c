--- src/afs/SOLARIS/osi_vfsops.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/SOLARIS/osi_vfsops.c
@@ -193,7 +193,7 @@ again:
 	}
     }
     if (tvp) {
-	AFS_FAST_HOLD(tvp);
+	osi_Assert(osi_vnhold(tvp) == 0);
 	mutex_enter(&AFSTOV(tvp)->v_lock);
 	AFSTOV(tvp)->v_flag |= VROOT;
 	mutex_exit(&AFSTOV(tvp)->v_lock);
