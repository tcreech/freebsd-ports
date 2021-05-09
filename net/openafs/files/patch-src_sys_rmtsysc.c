--- src/sys/rmtsysc.c.orig	2021-01-14 21:08:41 UTC
+++ src/sys/rmtsysc.c
@@ -135,14 +135,8 @@ rx_connection(afs_int32 * errorcode, char *syscall)
 
 /* WARNING: The calling program (i.e. klog) MUST be suid-root since we need to
  * do a setgroups(2) call with the new pag.... */
-#ifdef AFS_DUX40_ENV
-#pragma weak setpag = afs_setpag
 int
-afs_setpag(void)
-#else
-int
 setpag(void)
-#endif
 {
     struct rx_connection *conn;
     clientcred creds;
@@ -195,15 +189,8 @@ setpag(void)
 
 
 /* Remote pioctl(2) client routine */
-#ifdef AFS_DUX40_ENV
-#pragma weak pioctl = afs_pioctl
 int
-afs_pioctl(char *path, afs_int32 cmd, struct ViceIoctl *data,
-	   afs_int32 follow)
-#else
-int
 pioctl(char *path, afs_int32 cmd, struct ViceIoctl *data, afs_int32 follow)
-#endif
 {
     struct rx_connection *conn;
     clientcred creds;
@@ -218,10 +205,6 @@ pioctl(char *path, afs_int32 cmd, struct ViceIoctl *da
 	return errorcode;
     }
     (void)SetClientCreds(&creds, groups);
-#ifdef	AFS_OSF_ENV
-    if (!ins)
-	ins = 1;
-#endif
     if (!(inbuffer = malloc(ins)))
 	return (-1);		/* helpless here */
     if (data->in_size)
