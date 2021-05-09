--- src/config/param.sunx86_511.h.orig	2021-01-14 21:08:41 UTC
+++ src/config/param.sunx86_511.h
@@ -135,10 +135,6 @@
 
 #include <afs/afs_sysnames.h>
 
-#if 0
-#define AFS_GLOBAL_SUNLOCK	1	/* For global locking */
-#endif
-
 #define	AFS_3DISPARES		1	/* Utilize the 3 available disk inode 'spares' */
 
 /* so we get _IOW() when we include sys/ioctl.h */
