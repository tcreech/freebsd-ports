--- src/afs/AIX/osi_machdep.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/AIX/osi_machdep.h
@@ -31,8 +31,6 @@ typedef struct proc afs_proc_t;
 
 #define afs_bufferpages v.v_bufhw
 
-#define osi_vnhold(avc, r) do { VN_HOLD(AFSTOV(avc)); } while (0)
-
 #undef gop_lookupname
 #define	gop_lookupname(fnamep,segflg,followlink,compvpp) \
 	lookupvp((fnamep), (followlink), (compvpp), &afs_osi_cred)
