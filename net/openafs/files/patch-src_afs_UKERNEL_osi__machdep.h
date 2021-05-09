--- src/afs/UKERNEL/osi_machdep.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/UKERNEL/osi_machdep.h
@@ -49,11 +49,6 @@
 #undef gop_lookupname_user
 #define gop_lookupname_user(fnamep,segflg,followlink,compvpp) lookupname((fnamep),(segflg),(followlink),(compvpp))
 
-#define osi_vnhold(avc, r)  do { VN_HOLD(AFSTOV(avc)); } while(0)
-
-#if 0
-#define	afs_suser(x)	    suser(x)
-#endif
 /*
  * Global lock support.
  */
