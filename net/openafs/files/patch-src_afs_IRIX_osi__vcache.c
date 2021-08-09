--- src/afs/IRIX/osi_vcache.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/IRIX/osi_vcache.c
@@ -114,3 +114,9 @@ osi_PostPopulateVCache(struct vcache *avc) {
     avc->lastr = 0;
 }
 
+int
+osi_vnhold(struct vcache *avc)
+{
+    VN_HOLD(AFSTOV(avc));
+    return 0;
+}
