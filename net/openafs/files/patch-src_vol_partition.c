--- src/vol/partition.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/partition.c
@@ -39,10 +39,6 @@
 #endif
 
 #if !defined(AFS_SGI_ENV)
-#ifdef	AFS_OSF_ENV
-#include <sys/mount.h>
-#include <ufs/fs.h>
-#else /* AFS_OSF_ENV */
 #ifdef AFS_VFSINCL_ENV
 #define VFS
 #ifdef	AFS_SUN5_ENV
@@ -60,7 +56,6 @@
 #include <sys/fs.h>
 #endif
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 #include <sys/file.h>
 #ifdef	AFS_AIX_ENV
 #include <sys/vfs.h>
@@ -350,13 +345,6 @@ VCheckPartition(char *part, char *devname, int logging
 #endif /* AFS_NAMEI_ENV */
 #endif /* !AFS_LINUX20_ENV && !AFS_NT40_ENV */
 
-#if defined(AFS_DUX40_ENV) && !defined(AFS_NAMEI_ENV)
-    if (status.st_ino != ROOTINO) {
-	Log("%s is not a mounted file system; ignored.\n", part);
-	return 0;
-    }
-#endif
-
     VInitPartition(part, devname, status.st_dev);
 
     return 0;
@@ -675,7 +663,7 @@ VAttachPartitions(void)
     return errors;
 }
 #endif
-#if defined(AFS_DUX40_ENV) || defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
+#if defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
 int
 VAttachPartitions(void)
 {
@@ -1461,12 +1449,4 @@ AddPartitionToTable_r(struct DiskPartition64 *dp)
     DiskPartitionTable[dp->index] = dp;
 }
 
-#if 0
-static void
-DeletePartitionFromTable_r(struct DiskPartition64 *dp)
-{
-    opr_Assert(dp->index >= 0 && dp->index <= VOLMAXPARTS);
-    DiskPartitionTable[dp->index] = NULL;
-}
-#endif
 #endif /* AFS_DEMAND_ATTACH_FS */
