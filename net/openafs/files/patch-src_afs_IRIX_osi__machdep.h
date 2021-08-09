--- src/afs/IRIX/osi_machdep.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/IRIX/osi_machdep.h
@@ -36,8 +36,6 @@ typedef struct proc afs_proc_t;
 #undef gop_lookupname_user
 #define gop_lookupname_user(fnamep,segflg,followlink,compvpp) lookupname((fnamep),(segflg),(followlink),NULL,(compvpp), NULL)
 
-#define osi_vnhold(avc, r)  do { VN_HOLD(AFSTOV(avc)); } while(0)
-
 #ifdef AFS_SGI64_ENV
 #include <sys/flock.h>
 extern flid_t osi_flid;
