--- src/WINNT/afsd/cm_btree.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/cm_btree.c
@@ -2147,9 +2147,6 @@ long cm_BPlusDirBuildTree(cm_scache_t *scp, cm_user_t 
 
     bplus_build_time += (end.QuadPart - start.QuadPart);
 
-#if 0
-    cm_BPlusDirEnumTest(scp, 1);
-#endif
     return rc;
 }
 
