--- src/afs/DARWIN/osi_machdep.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/DARWIN/osi_machdep.h
@@ -136,7 +136,6 @@ extern int hz;
 typedef struct ucred afs_ucred_t;
 typedef struct proc afs_proc_t;
 
-#define osi_vnhold(avc,r)       VN_HOLD(AFSTOV(avc))
 #define VN_HOLD(vp) darwin_vn_hold(vp)
 #define VN_RELE(vp) vrele(vp)
 
