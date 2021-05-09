--- src/sys/sys_prototypes.h.orig	2021-01-14 21:08:41 UTC
+++ src/sys/sys_prototypes.h
@@ -29,11 +29,7 @@ extern int lpioctl(char *, int, void *, int);
 struct ViceIoctl;
 extern int pioctl(char *path, afs_int32 cmd, struct ViceIoctl *data,
 		  afs_int32 follow);
-#ifdef AFS_DUX40_ENV
-extern int afs_setpag(void);
-#else
 extern int setpag(void);
-#endif
 
 /* rmtsysnet.c */
 extern void inparam_conversion(afs_int32, char *, afs_int32);
