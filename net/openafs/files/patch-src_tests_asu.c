--- src/tests/asu.c.orig	2021-01-14 21:08:41 UTC
+++ src/tests/asu.c
@@ -86,13 +86,6 @@ main(int argc, char **argv)
 	setegid(gid);
 	seteuid(uid);
     }
-#if 0
-    if (k_hasafs()) {
-	int ret = k_setpag();
-	if (ret < 0)
-	    warn("k_setpag");
-    }
-#endif
 
     execvp(prog, &argv[2]);
 
