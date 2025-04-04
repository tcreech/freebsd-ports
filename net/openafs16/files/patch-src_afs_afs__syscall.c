--- src/afs/afs_syscall.c.orig	2016-03-16 14:31:22 UTC
+++ src/afs/afs_syscall.c
@@ -27,6 +27,10 @@
 #if !defined(AFS_HPUX110_ENV) && !defined(AFS_DARWIN_ENV)
 #include "netinet/in_var.h"
 #endif
+#if defined(AFS_FBSD80_ENV)
+#include <sys/param.h>
+#include <sys/jail.h>
+#endif
 #endif /* !defined(UKERNEL) */
 
 #if (defined(AFS_AIX51_ENV) && defined(AFS_64BIT_KERNEL)) || defined(AFS_HPUX_64BIT_ENV) || defined(AFS_SUN57_64BIT_ENV) || (defined(AFS_SGI_ENV) && (_MIPS_SZLONG==64)) || defined(NEED_IOCTL32)
@@ -477,6 +481,10 @@ afs3_syscall(struct thread *p, void *arg
 	long parm6;
     } *uap = (struct a *)args;
     long *retval;
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_SET(TD_TO_VNET(curthread));
+#endif
+
 #elif defined(AFS_NBSD40_ENV)
 int
 afs3_syscall(struct lwp *p, void *args)
@@ -868,6 +876,11 @@ Afs_syscall()
 #ifdef AFS_LINUX20_ENV
     code = -code;
 #endif
+
+#if defined(AFS_FBSD80_ENV)
+    CURVNET_RESTORE();
+#endif
+
     return code;
 }
 #endif /* AFS_SGI_ENV */
