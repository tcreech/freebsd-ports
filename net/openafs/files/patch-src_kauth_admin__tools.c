--- src/kauth/admin_tools.c.orig	2021-01-14 21:08:41 UTC
+++ src/kauth/admin_tools.c
@@ -1546,47 +1546,6 @@ ForgetTicket(struct cmd_syndesc *as, void *arock)
 {
     afs_int32 code;
 
-#ifdef notdef
-    struct ktc_principal server;
-
-    if (as->parms[0].items) {
-	char *name = as->parms[0].items->data;
-	code =
-	    ka_ParseLoginName(name, server.name, server.instance,
-			      server.cell);
-	if (code) {
-	    afs_com_err(whoami, code, "couldn't interpret name '%s'", name);
-	    return code;
-	}
-	if (server.cell[0] == 0) {
-	    if (code = DefaultCell())
-		return code;
-	    strcpy(server.cell, cell);
-	} else {
-	    code = ka_ExpandCell(server.cell, server.cell, 0 /*local */ );
-	    if (code) {
-		afs_com_err(whoami, code, "Can't expand cell name");
-		return code;
-	    }
-	}
-	code = ktc_ForgetToken(&server);
-	if (code) {
-	    afs_com_err(whoami, code, "couldn't remove tokens for %s",
-		    PrintedPrincipal(&server));
-	    return code;
-	}
-    } else {
-	if (!as->parms[1].items) {
-	    fprintf(stderr, "Must specify server name or -all\n");
-	    return KABADCMD;
-	}
-	code = ktc_ForgetAllTokens();
-	if (code) {
-	    afs_com_err(whoami, code, "couldn't delete all tokens");
-	    return code;
-	}
-    }
-#endif
     code = ktc_ForgetAllTokens();
     if (code) {
 	afs_com_err(whoami, code, "couldn't delete all tokens");
@@ -1790,10 +1749,6 @@ ka_AdminInteractive(int cmd_argc, char *cmd_argv[])
 
     ts = cmd_CreateSyntax("forgetticket", ForgetTicket, NULL, 0,
 			  "delete user's tickets");
-#ifdef notdef
-    cmd_AddParm(ts, "-name", CMD_SINGLE, (CMD_OPTIONAL | CMD_HIDE),
-		"name of server");
-#endif
     cmd_AddParm(ts, "-all", CMD_FLAG, CMD_OPTIONAL, "delete all tickets");
 
     ts = cmd_CreateSyntax("listtickets", ListTickets, NULL, 0,
