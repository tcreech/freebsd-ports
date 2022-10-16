--- src/vol/devname.c.orig	2018-04-06 01:21:12 UTC
+++ src/vol/devname.c
@@ -169,7 +169,11 @@ vol_DevName(dev_t adev, char *wpath)
 	}
 #if !defined(AFS_SGI_XFS_IOPS_ENV) && !defined(AFS_LINUX22_ENV) && !defined(AFS_DARWIN_ENV)
 	if ((status.st_ino !=
+#ifdef AFS_FBSD120_ENV
+	     UFS_ROOTINO) /*|| ((status.st_mode & S_IFMT) != S_IFBLK) */ ) {
+#else
 	     ROOTINO) /*|| ((status.st_mode & S_IFMT) != S_IFBLK) */ ) {
+#endif //AFS_FBSD120_ENV
 	    continue;
 	}
 #endif
