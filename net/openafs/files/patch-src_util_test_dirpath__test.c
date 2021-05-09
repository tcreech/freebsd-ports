--- src/util/test/dirpath_test.c.orig	2021-01-14 21:08:41 UTC
+++ src/util/test/dirpath_test.c
@@ -62,8 +62,10 @@ main(int argc, char *argv[])
 	   AFSDIR_SERVER_THISCELL_FILEPATH);
     printf("AFSDIR_SERVER_CELLSERVDB_FILEPATH  = %s\n",
 	   AFSDIR_SERVER_CELLSERVDB_FILEPATH);
-    printf("AFSDIR_SERVER_KEY_FILEPATH AFSDIR = %s\n",
+    printf("AFSDIR_SERVER_KEY_FILEPATH = %s\n",
 	   AFSDIR_SERVER_KEY_FILEPATH);
+    printf("AFSDIR_SERVER_EXT_KEY_FILEPATH = %s\n",
+	   AFSDIR_SERVER_EXT_KEY_FILEPATH);
     printf("AFSDIR_SERVER_ULIST_FILEPATH = %s\n",
 	   AFSDIR_SERVER_ULIST_FILEPATH);
     printf("AFSDIR_SERVER_NOAUTH_FILEPATH = %s\n",
