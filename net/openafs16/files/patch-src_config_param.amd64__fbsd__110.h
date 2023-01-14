--- src/config/param.amd64_fbsd_110.h.orig	2017-02-17 12:40:03 UTC
+++ src/config/param.amd64_fbsd_110.h
@@ -8,7 +8,7 @@
 #define AFSLITTLE_ENDIAN    1
 #define AFS_HAVE_FFS        1	/* Use system's ffs. */
 #define AFS_HAVE_STATVFS    1	/* System doesn't support statvfs */
-#define AFS_VM_RDWR_ENV	    1	/* read/write implemented via VM */
+//#define AFS_VM_RDWR_ENV	    1	/* read/write implemented via VM */
 
 
 #ifndef UKERNEL
