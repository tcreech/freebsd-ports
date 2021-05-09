--- src/afs/HPUX/osi_machdep.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/HPUX/osi_machdep.h
@@ -29,7 +29,6 @@ extern struct timeval time;
 typedef struct ucred afs_ucred_t;
 typedef proc_t afs_proc_t;
 
-#define osi_vnhold(avc, r)  do { VN_HOLD(AFSTOV(avc)); } while(0)
 #define gop_rdwr(rw,gp,base,len,offset,segflg,unit,aresid) \
   vn_rdwr((rw),(gp),(base),(len),(offset),(segflg),(unit),(aresid),0)
 #define gop_lookupname(fnamep,segflg,followlink,compvpp) \
