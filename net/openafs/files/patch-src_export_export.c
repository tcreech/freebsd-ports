--- src/export/export.c.orig	2021-01-14 21:08:41 UTC
+++ src/export/export.c
@@ -165,9 +165,6 @@ import_kfunc(struct k_func * kfp)
 {
     sym_t *sym;
     int i, pri;
-#if 0
-    static caddr_t *g_toc;
-#endif
 
     if (!myg_toc) {
 #ifdef __XCOFF64__
