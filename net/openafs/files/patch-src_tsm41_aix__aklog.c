--- src/tsm41/aix_aklog.c.orig	2021-01-14 21:08:41 UTC
+++ src/tsm41/aix_aklog.c
@@ -179,39 +179,6 @@ aklog_authenticate(char *userName, char *response, int
 		    str);
 	return AUTH_FAILURE; /* NOTFOUND? */
     }
-
-#if 0
-    /*
-     * Local hack - if the person has a file in their home
-     * directory called ".xlog", read that for a list of
-     * extra cells to authenticate to
-     */
-
-    if ((pwd = getpwuid(getuid())) != NULL) {
-	struct stat sbuf;
-	FILE *f;
-	char fcell[100], xlog_path[512];
-
-	strcpy(xlog_path, pwd->pw_dir);
-	strcat(xlog_path, "/.xlog");
-
-	if ((stat(xlog_path, &sbuf) == 0) &&
-	    ((f = fopen(xlog_path, "r")) != NULL)) {
-
-	    while (fgets(fcell, 100, f) != NULL) {
-		int auth_status;
-
-		fcell[strlen(fcell) - 1] = '\0';
-
-		auth_status = auth_to_cell(context, userName, fcell, NULL);
-		if (status == AKLOG_SUCCESS)
-		    status = auth_status;
-		else
-		    status = AKLOG_SOMETHINGSWRONG;
-	    }
-	}
-    }
-#endif
     return AUTH_SUCCESS;
 }
 
