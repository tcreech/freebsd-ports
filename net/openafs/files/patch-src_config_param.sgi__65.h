--- src/config/param.sgi_65.h.orig	2021-01-14 21:08:41 UTC
+++ src/config/param.sgi_65.h
@@ -62,16 +62,6 @@
 #define AFS_SGI_XFS_IOPS_ENV 	1	/* turns on XFS inode ops. */
 #define AFS_64BIT_IOPS_ENV	1	/* inode ops expect 64 bit inodes */
 
-/* Vnode size and access differs between Octane and Origin. The number
- * can be used to indicate which altername vnodeX_t to use for future
- * changes.
- */
-#ifdef notdef
-/* SGI may have cleared this problem up. */
-#define AFS_SGI_VNODE_GLUE	1
-#endif
-
-
 /* File system entry (used if mount.h doesn't define MOUNT_AFS */
 #define AFS_MOUNT_AFS	 "afs"
 
