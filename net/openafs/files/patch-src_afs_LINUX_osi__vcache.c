--- src/afs/LINUX/osi_vcache.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/LINUX/osi_vcache.c
@@ -103,8 +103,11 @@ osi_TryEvictVCache(struct vcache *avc, int *slept, int
 	&& avc->opens == 0) {
 	struct inode *ip = AFSTOV(avc);
 
+	if (osi_vnhold(avc) != 0) {
+	    /* Can't grab a ref on avc; bail out. */
+	    return 0;
+	}
 	*slept = 1;
-	AFS_FAST_HOLD(avc);
 	ReleaseWriteLock(&afs_xvcache);
 	AFS_GUNLOCK();
 
@@ -235,4 +238,11 @@ osi_ResetRootVCache(afs_uint32 volid)
 out:
     crfree(credp);
     afs_DestroyReq(treq);
+}
+
+int
+osi_vnhold(struct vcache *avc)
+{
+    VN_HOLD(AFSTOV(avc));
+    return 0;
 }
