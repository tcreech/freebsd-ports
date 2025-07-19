--- src/afs/afs_prototypes.h.orig	2024-10-03 22:32:45 UTC
+++ src/afs/afs_prototypes.h
@@ -784,7 +784,7 @@ extern int afs_xioctl(afs_proc_t *p, struct ioctl_args
 #elif defined(AFS_DAWRIN_ENV) & !defined(AFS_DARWIN80)
 extern int afs_xioctl(afs_proc_t *p, struct ioctl_args *uap, register_t *retval);
 #elif defined(AFS_FBSD_ENV)
-extern int afs_xioctl(struct thread *td, struct ioctl_args *uap, register_t *retval);
+//extern int afs_xioctl(struct thread *td, struct ioctl_args *uap, register_t *retval);
 #elif defined(AFS_NBSD_ENV)
 extern int afs_xioctl(afs_proc_t *p, const struct sys_ioctl_args *uap, register_t *retval);
 #elif defined(AFS_XBSD_ENV)
