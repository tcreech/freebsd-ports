--- src/afs/afs_prototypes.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/afs_prototypes.h
@@ -928,9 +928,6 @@ extern int copyin_afs_ioctl(caddr_t cmarg, struct afs_
 #if defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
 #ifdef AFS_DARWIN100_ENV
 extern int afs3_syscall(afs_proc_t *p, void *args, unsigned int *retval);
-#elif (defined(AFS_FBSD90_ENV) || defined(AFS_FBSD82_ENV)) && (__FreeBSD_version < 900044)
-/* afs3_syscall prototype is in sys/sysproto.h
-   Yes, they put it in, then took it out again (renamed with a sys_ prefix) */
 #elif defined(AFS_FBSD_ENV)
 extern int afs3_syscall(struct thread *p, void *args);
 #elif defined(AFS_NBSD50_ENV)
