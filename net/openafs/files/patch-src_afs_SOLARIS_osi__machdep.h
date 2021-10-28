--- src/afs/SOLARIS/osi_machdep.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/SOLARIS/osi_machdep.h
@@ -54,19 +54,6 @@ local_osi_Time()
 #define osi_Time() (hrestime.tv_sec)
 #endif
 
-#define osi_vnhold(avc, r)  do {    \
-    struct vnode *vp = AFSTOV(avc); \
-    uint_t prevcount;               \
-                                    \
-    mutex_enter(&vp->v_lock);       \
-    prevcount = vp->v_count++;      \
-    mutex_exit(&vp->v_lock);        \
-                                    \
-    if (prevcount == 0) {           \
-	VFS_HOLD(afs_globalVFS);    \
-    }                               \
-} while(0)
-
 #define gop_rdwr(rw,gp,base,len,offset,segflg,ioflag,ulimit,cr,aresid) \
   vn_rdwr((rw),(gp),(base),(len),(offset),(segflg),(ioflag),(ulimit),(cr),(aresid))
 #define gop_lookupname(fnamep,segflg,followlink,compvpp) \
