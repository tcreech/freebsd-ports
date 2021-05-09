--- src/afs/DARWIN/osi_vcache.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/DARWIN/osi_vcache.c
@@ -110,3 +110,10 @@ osi_PostPopulateVCache(struct vcache *avc) {
    vSetType(avc, VNON);
 #endif
 }
+
+int
+osi_vnhold(struct vcache *avc)
+{
+    VN_HOLD(AFSTOV(avc));
+    return 0;
+}
