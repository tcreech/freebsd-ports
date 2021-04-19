--- src/afs/afs_osi_alloc.c.orig	2017-06-24 14:40:54 UTC
+++ src/afs/afs_osi_alloc.c
@@ -53,7 +53,7 @@ afs_osi_Alloc(size_t size)
 #ifdef AFS_LINUX20_ENV
     return osi_linux_alloc(size, 1);
 #elif defined(AFS_FBSD_ENV)
-    return osi_fbsd_alloc(size, 1);
+    return osi_fbsd_alloc(size, 0);
 #else
     return AFS_KALLOC(size);
 #endif
