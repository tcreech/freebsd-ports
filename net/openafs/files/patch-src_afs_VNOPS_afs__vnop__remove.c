--- src/afs/VNOPS/afs_vnop_remove.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/VNOPS/afs_vnop_remove.c
@@ -423,7 +423,7 @@ afs_remunlink(struct vcache *avc, int doit)
 #if defined(AFS_DARWIN_ENV) && !defined(AFS_DARWIN80_ENV)
 	    VREF(AFSTOV(avc));
 #else
-	    AFS_FAST_HOLD(avc);
+	    osi_Assert(osi_vnhold(avc) == 0);
 #endif
 
 	    /* We'll only try this once. If it fails, just release the vnode.
