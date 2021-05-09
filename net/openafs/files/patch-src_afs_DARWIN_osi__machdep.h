--- src/afs/DARWIN/osi_machdep.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/DARWIN/osi_machdep.h
@@ -88,11 +88,6 @@ enum vcexcl { EXCL, NONEXCL };
 #define vput vnode_rele
 #define vref vnode_ref
 #define vattr vnode_attr
-#if 0
-#define vn_lock(v, unused1, unused2) vnode_get((v))
-#define VOP_LOCK(v, unused1, unused2) vnode_get((v))
-#define VOP_UNLOCK(v, unused1, unused2) vnode_put((v))
-#endif
 
 #define va_size va_data_size
 #define va_atime va_access_time
@@ -141,7 +136,6 @@ extern int hz;
 typedef struct ucred afs_ucred_t;
 typedef struct proc afs_proc_t;
 
-#define osi_vnhold(avc,r)       VN_HOLD(AFSTOV(avc))
 #define VN_HOLD(vp) darwin_vn_hold(vp)
 #define VN_RELE(vp) vrele(vp)
 
@@ -203,15 +197,6 @@ extern struct lock__bsd__ afs_global_lock;
 #define SPLVAR
 #define NETPRI
 #define USERPRI
-#if 0
-#undef SPLVAR
-#define SPLVAR int x
-#undef NETPRI
-#define NETPRI x=splnet()
-#undef USERPRI
-#define USERPRI splx(x)
-#endif
-
 #define AFS_APPL_UFS_CACHE 1
 #define AFS_APPL_HFS_CACHE 2
 #define AFS_APPL_APFS_CACHE 3
