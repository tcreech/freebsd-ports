--- src/afs/UKERNEL/sysincludes.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/UKERNEL/sysincludes.h
@@ -82,23 +82,6 @@
 #include  <arpa/inet.h>
 #endif /* AFS_USR_HPUX_ENV */
 
-#ifdef AFS_USR_OSF_ENV
-#ifdef KERNEL
-#undef KERNEL
-#define AFS_USR_UNDEF_KERNEL_ENV 1
-#endif
-#include  <sys/param.h>
-#include  <sys/socket.h>
-#include  <net/if.h>
-#include  <sys/file.h>
-#include  <sys/ioctl.h>
-#include  <sys/stat.h>
-#include  <sys/fcntl.h>
-#include  <netinet/in.h>
-#include  <netdb.h>
-#include  <arpa/inet.h>
-#endif /* AFS_USR_OSF_ENV */
-
 #ifdef AFS_USR_LINUX22_ENV
 #include  <sys/ioctl.h>		/* _IOW() */
 #include  <sys/uio.h>		/* struct iovec */
@@ -256,11 +239,7 @@ typedef unsigned int fsblkcnt_t;
 
 enum usr_vcexcl { usr_NONEXCL, usr_EXCL };
 typedef long offset_t;
-#ifdef AFS_USR_OSF_ENV
-typedef int usr_ino_t;
-#else /* AFS_USR_OSF_ENV */
 typedef long usr_ino_t;
-#endif /* AFS_USR_OSF_ENV */
 
 #if defined(AFS_USR_AIX_ENV) || defined(AFS_USR_SGI_ENV)
 #define SYS_setgroups		101
@@ -1242,11 +1221,7 @@ extern struct usr_in_ifaddr *usr_in_ifaddr;
 #endif /* UKERNEL */
 
 struct min_direct {
-#if defined(AFS_USR_OSF_ENV)
-    unsigned int d_fileno;
-#else				/* AFS_OFS_ENV || AFS_USR_OSF_ENV */
     unsigned long d_fileno;
-#endif				/* AFS_OFS_ENV || AFS_USR_OSF_ENV */
     unsigned short d_reclen;
     unsigned short d_namlen;
 };
