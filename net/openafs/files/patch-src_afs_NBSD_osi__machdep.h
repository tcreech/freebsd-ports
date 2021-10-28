--- src/afs/NBSD/osi_machdep.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/NBSD/osi_machdep.h
@@ -50,7 +50,6 @@ typedef char * caddr_t;
 /* vnode */
 #define VN_HOLD(vp)	(vget(vp, 0))
 #define VN_RELE(vp)	(vrele(vp))
-#define osi_vnhold(avc, r) (VN_HOLD(AFSTOV(avc)))
 
 #define va_nodeid	va_fileid
 
