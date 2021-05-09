--- src/kauth/kpasswd.c.orig	2021-01-14 21:08:41 UTC
+++ src/kauth/kpasswd.c
@@ -155,21 +155,6 @@ read_pass(char *passwd, int len, char *prompt, int ver
     return code;
 }
 
-#if 0
-static int
-password_ok(char *newpw, int *insist)
-{
-    if (insist == 0) {
-	/* see if it is reasonable, but don't get so obnoxious */
-	/* FIXME: null pointer derefence!!! */
-	(*insist)++;		/* so we don't get called again */
-	if (strlen(newpw) < 6)
-	    return 0;
-    }
-    return 1;			/* lie about it */
-}
-#endif
-
 static char rn[] = "kpasswd";	/* Routine name */
 static int Pipe = 0;		/* reading from a pipe */
 
@@ -474,13 +459,6 @@ CommandProc(struct cmd_syndesc *as, void *arock)
 	    memset(passwd, 0, sizeof(passwd));
 	    code = ka_GetAdminToken(pw->pw_name, instance, realm, &key, ADMIN_LIFETIME, &token,	/*!new */
 				    0);
-#ifdef notdef
-	    /* the folks in testing really *hate* this message */
-	    if (code == 0) {
-		fprintf(stderr,
-			"Warning: only the first 8 characters of your old password were significant.\n");
-	    }
-#endif
 	    if (code == 0) {
 		if (dess2k == -1)
 		    dess2k = 0;
