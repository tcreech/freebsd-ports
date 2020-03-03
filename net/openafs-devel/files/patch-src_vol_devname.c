--- src/vol/devname.c.orig	2020-02-21 20:17:12 UTC
+++ src/vol/devname.c
@@ -165,7 +165,11 @@ vol_DevName(dev_t adev, char *wpath)
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
