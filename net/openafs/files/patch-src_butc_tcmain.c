--- src/butc/tcmain.c.orig	2021-01-14 21:08:41 UTC
+++ src/butc/tcmain.c
@@ -66,7 +66,7 @@
 #define TL_PREFIX  "TL"
 #define CFG_PREFIX "CFG"
 
-struct ubik_client *cstruct;
+extern struct ubik_client *cstruct;
 FILE *logIO, *ErrorlogIO, *centralLogIO, *lastLogIO;
 char lFile[AFSDIR_PATH_MAX];
 char logFile[AFSDIR_PATH_MAX + 256];
@@ -237,30 +237,13 @@ atocl(char *numstring, char crunit, afs_int32 *number)
 	total *= 1024.0;
 
     total += 0.5;		/* Round up */
-    if ((total > 0x7fffffff) || (total < 0))	/* Don't go over 2G */
-	total = 0x7fffffff;
+    if ((total >= 2147483648.0) || (total < 0))	/* Don't go over 2G */
+	*number = 2147483647;
+    else
+	*number = total;
 
-    *number = total;
     return (0);
 }
-
-/* replace last two ocurrences of / by _ */
-#if 0
-static int
-stringReplace(char *name)
-{
-    char *pos;
-    char buffer[256];
-
-    pos = strrchr(name, '/');
-    *pos = '_';
-    strcpy(buffer, pos);
-    pos = strrchr(name, '/');
-    *pos = '\0';
-    strcat(name, buffer);
-    return 0;
-}
-#endif
 
 static int
 stringNowReplace(char *logFile, char *deviceName)
