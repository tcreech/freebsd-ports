--- src/afs/HPUX/osi_vcache.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/HPUX/osi_vcache.c
@@ -50,3 +50,9 @@ osi_PostPopulateVCache(struct vcache *avc) {
     vSetType(avc, VREG);
 }
 
+int
+osi_vnhold(struct vcache *avc)
+{
+    VN_HOLD(AFSTOV(avc));
+    return 0;
+}
