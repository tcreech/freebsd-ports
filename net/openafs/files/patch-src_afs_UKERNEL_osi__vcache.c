--- src/afs/UKERNEL/osi_vcache.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/UKERNEL/osi_vcache.c
@@ -47,3 +47,9 @@ osi_PostPopulateVCache(struct vcache *avc) {
     vSetType(avc, VREG);
 }
 
+int
+osi_vnhold(struct vcache *avc)
+{
+    VN_HOLD(AFSTOV(avc));
+    return 0;
+}
