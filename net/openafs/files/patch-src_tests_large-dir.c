--- src/tests/large-dir.c.orig	2021-01-14 21:08:41 UTC
+++ src/tests/large-dir.c
@@ -121,10 +121,6 @@ creat_files(const char *dirname, int count)
     closedir(d);
     if (stat(".", &sb) < 0)
 	err(1, "stat .");
-#if 0
-    if (sb.st_size != 2048)
-	errx(1, "size != 2048");
-#endif
     return 0;
 }
 
