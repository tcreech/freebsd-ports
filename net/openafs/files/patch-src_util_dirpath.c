--- src/util/dirpath.c.orig	2021-01-14 21:08:41 UTC
+++ src/util/dirpath.c
@@ -396,6 +396,9 @@ initDirPathArray(void)
     pathp = dirPathArray[AFSDIR_SERVER_RXKAD_KEYTAB_FILEPATH_ID];
     AFSDIR_SERVER_FILEPATH(pathp, AFSDIR_SERVER_ETC_DIR, AFSDIR_RXKAD_KEYTAB_FILE);
 
+    pathp = dirPathArray[AFSDIR_SERVER_EXT_KEY_FILEPATH_ID];
+    AFSDIR_SERVER_FILEPATH(pathp, AFSDIR_SERVER_ETC_DIR, AFSDIR_EXT_KEY_FILE);
+
     /* client file paths */
 #ifdef AFS_NT40_ENV
     strcpy(dirPathArray[AFSDIR_CLIENT_THISCELL_FILEPATH_ID],
