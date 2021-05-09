--- src/ptserver/testpt.c.orig	2021-01-14 21:08:41 UTC
+++ src/ptserver/testpt.c
@@ -445,18 +445,6 @@ TestManyMembers(struct cmd_syndesc *as, void *arock)
 	    if ((code == 0) && (callerId == ANONYMOUSID))
 		code = PRNOENT;
 	}
-#if 0				/* don't create user */
-	if (code == PRNOENT) {
-	    callerId = 0;
-	    code = pr_CreateUser(callerName, &callerId);
-	    if (code) {
-		afs_com_err(whoami, code, "can't create caller %s", callerName);
-		exit(5);
-	    }
-	    printf("Creating caller %s (%di)\n", callerName, callerId);
-	}
-	/* else */
-#endif
 	if (code) {
 	    afs_com_err(whoami, code, "can't find caller %s", callerName);
 	    exit(6);
