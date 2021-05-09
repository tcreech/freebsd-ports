--- src/tests/large-dir2.c.orig	2021-01-14 21:08:41 UTC
+++ src/tests/large-dir2.c
@@ -64,10 +64,6 @@ creat_files(const char *dirname, int count)
 	err(1, "chdir %s", dirname);
     if (stat(".", &sb) < 0)
 	err(1, "stat .");
-#if 0
-    if (sb.st_size != 2048)
-	errx(1, "size != 2048");
-#endif
     for (i = 0; i < count; ++i) {
 	char fname[256];
 	int len;
