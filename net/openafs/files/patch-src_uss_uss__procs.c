--- src/uss/uss_procs.c.orig	2021-01-14 21:08:41 UTC
+++ src/uss/uss_procs.c
@@ -659,12 +659,12 @@ afs_int32
 uss_procs_PickADir(char *path, char *cp)
 {				/*uss_procs_PickADir */
 
-    char cd[300];		/*Current  directory for search */
+    char cd[uss_DirPoolLen];		/*Current  directory for search */
 
     int i, count, MinIndex = 0, mina = 10000;
     struct dirent *dp;
     DIR *dirp;
-    char dirname[301];
+    char dirname[uss_DirPoolLen*2];
 
     if (uss_NumGroups == 0) {
 	fprintf(stderr, "%s: No choice yet given to replace $AUTO\n",
