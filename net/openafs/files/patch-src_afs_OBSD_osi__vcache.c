--- src/afs/OBSD/osi_vcache.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/OBSD/osi_vcache.c
@@ -64,3 +64,8 @@ osi_PostPopulateVCache(struct vcache *avc) {
     vSetType(avc, VREG);
 }
 
+int
+osi_vnhold(struct vcache *avc)
+{
+    return afs_vget(AFSTOV(avc), 0);
+}
