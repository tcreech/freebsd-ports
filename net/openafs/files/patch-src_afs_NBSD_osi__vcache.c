--- src/afs/NBSD/osi_vcache.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/NBSD/osi_vcache.c
@@ -74,3 +74,9 @@ osi_PostPopulateVCache(struct vcache *avc)
     vSetType(avc, VREG);
 }
 
+int
+osi_vnhold(struct vcache *avc)
+{
+    VN_HOLD(AFSTOV(avc));
+    return 0;
+}
