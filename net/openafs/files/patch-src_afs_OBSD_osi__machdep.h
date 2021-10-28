--- src/afs/OBSD/osi_machdep.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/OBSD/osi_machdep.h
@@ -45,7 +45,6 @@
 /* vnode */
 #define VN_HOLD(vp)	afs_vget((vp), 0)
 #define VN_RELE(vp)	vrele(vp)
-#define osi_vnhold(avc, r) afs_vget(AFSTOV(avc), 0)
 #define va_nodeid	va_fileid
 #define vnode_t		struct vnode
 
