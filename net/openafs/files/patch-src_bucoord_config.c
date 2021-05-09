--- src/bucoord/config.c.orig	2021-01-14 21:08:41 UTC
+++ src/bucoord/config.c
@@ -16,26 +16,6 @@
 
 struct bc_config *bc_globalConfig;
 
-#if 0
-static int
-TrimLine(char *abuffer, afs_int32 *aport)
-{
-    int tc;
-    char garb[100];
-
-    *aport = 0;
-    sscanf(abuffer, "%s %u", garb, aport);
-    while ((tc = *abuffer)) {
-	if (tc == ' ') {
-	    *abuffer = 0;
-	    return 0;
-	}
-	abuffer++;
-    }
-    return 0;
-}
-#endif
-
 FILE *
 bc_open(struct bc_config *aconfig, char *aname, char *aext, char *amode)
 {
