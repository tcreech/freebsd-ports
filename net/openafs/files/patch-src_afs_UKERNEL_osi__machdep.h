--- src/afs/UKERNEL/osi_machdep.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/UKERNEL/osi_machdep.h
@@ -49,8 +49,6 @@
 #undef gop_lookupname_user
 #define gop_lookupname_user(fnamep,segflg,followlink,compvpp) lookupname((fnamep),(segflg),(followlink),(compvpp))
 
-#define osi_vnhold(avc, r)  do { VN_HOLD(AFSTOV(avc)); } while(0)
-
 /*
  * Global lock support.
  */
