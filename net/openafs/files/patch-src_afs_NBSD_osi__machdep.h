--- src/afs/NBSD/osi_machdep.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/NBSD/osi_machdep.h
@@ -44,17 +44,12 @@ typedef char * caddr_t;
 #define osi_vfs_bsize	mnt_stat.f_bsize
 #define osi_vfs_iosize	mnt_stat.f_iosize
 #define osi_vfs_fsid	mnt_stat.f_fsid
-#if 0
-#define vfs_bsize	mnt_stat.f_bsize
-#define vfs_fsid	mnt_stat.f_fsid
-#endif
 #define vfs_vnodecovered mnt_vnodecovered
 #define v_vfsp		v_mount
 
 /* vnode */
 #define VN_HOLD(vp)	(vget(vp, 0))
 #define VN_RELE(vp)	(vrele(vp))
-#define osi_vnhold(avc, r) (VN_HOLD(AFSTOV(avc)))
 
 #define va_nodeid	va_fileid
 
