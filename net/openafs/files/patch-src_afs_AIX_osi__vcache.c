--- src/afs/AIX/osi_vcache.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/AIX/osi_vcache.c
@@ -75,3 +75,9 @@ osi_PostPopulateVCache(struct vcache *avc) {
     avc->v.v_gnode->gn_vnode = &avc->v;
 }
 
+int
+osi_vnhold(struct vcache *avc)
+{
+    VN_HOLD(AFSTOV(avc));
+    return 0;
+}
