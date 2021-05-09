--- src/bozo/bosoprocs.c.orig	2021-01-14 21:08:41 UTC
+++ src/bozo/bosoprocs.c
@@ -1266,8 +1266,10 @@ struct bozo_bosEntryStats bozo_bosEntryStats[] = {
     {NULL, 1, 1, 0700, 07},	/* AFSDIR_SERVER_DB_DIRPATH     */
     {NULL, 1, 1, 0700, 07},	/* AFSDIR_SERVER_LOCAL_DIRPATH  */
     {NULL, 0, 1, 0600, 07},	/* AFSDIR_SERVER_KEY_FILEPATH   */
-    {NULL, 0, 1, 0600, 03}
-};				/* AFSDIR_SERVER_ULIST_FILEPATH */
+    {NULL, 0, 1, 0600, 03},	/* AFSDIR_SERVER_ULIST_FILEPATH */
+    {NULL, 0, 1, 0600, 07},	/* AFSDIR_SERVER_RXKAD_KEYTAB_FILEPATH   */
+    {NULL, 0, 1, 0600, 07}	/* AFSDIR_SERVER_EXT_KEY_FILEPATH   */
+};
 int bozo_nbosEntryStats =
     sizeof(bozo_bosEntryStats) / sizeof(bozo_bosEntryStats[0]);
 
@@ -1290,6 +1292,8 @@ initBosEntryStats(void)
     bozo_bosEntryStats[6].path = AFSDIR_SERVER_LOCAL_DIRPATH;
     bozo_bosEntryStats[7].path = AFSDIR_SERVER_KEY_FILEPATH;
     bozo_bosEntryStats[8].path = AFSDIR_SERVER_ULIST_FILEPATH;
+    bozo_bosEntryStats[9].path = AFSDIR_SERVER_RXKAD_KEYTAB_FILEPATH;
+    bozo_bosEntryStats[10].path = AFSDIR_SERVER_EXT_KEY_FILEPATH;
 
     return 0;
 }
@@ -1352,7 +1356,6 @@ DirAccessOK(void)
 		     (~e->proPerm & 0777),
 		     e->rootOwner ? ", owned by root" : "");
 	    result = 0;
-	    break;
 	}
     }
 
