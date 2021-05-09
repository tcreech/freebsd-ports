--- src/tests/rename-under-feet.c.orig	2021-01-14 21:08:41 UTC
+++ src/tests/rename-under-feet.c
@@ -102,10 +102,6 @@ child_chdir(const char *path)
 	    err(1, "write");
 	while (!term_sig)
 	    pause();
-#if 0
-	if (getcwd(buf, sizeof(buf)) == NULL)
-	    err(1, "getcwd");
-#endif
 	fp = fdopen(4, "w");
 	if (fp != NULL)
 	    fprintf(fp, "child: cwd = %s\n", buf);
