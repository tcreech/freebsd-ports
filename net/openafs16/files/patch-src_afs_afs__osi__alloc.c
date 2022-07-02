--- src/afs/afs_osi_alloc.c.orig	2016-11-30 20:06:42 UTC
+++ src/afs/afs_osi_alloc.c
@@ -60,7 +60,7 @@ afs_osi_Alloc(size_t x)
 #ifdef AFS_LINUX20_ENV
     return osi_linux_alloc(x, 1);
 #elif defined(AFS_FBSD_ENV)
-    return osi_fbsd_alloc(x, 1);
+    return osi_fbsd_alloc(x, 0);
 #else
     size = x;
     tm = (struct osimem *)AFS_KALLOC(size);
