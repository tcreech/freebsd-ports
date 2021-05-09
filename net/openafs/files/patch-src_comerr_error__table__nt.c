--- src/comerr/error_table_nt.c.orig	2021-01-14 21:08:41 UTC
+++ src/comerr/error_table_nt.c
@@ -863,14 +863,6 @@ yyparse(YYPARSE_PARAM)
 
   yyerrdefault:		/* current state does not do anything special for the error token. */
 
-#if 0
-    /* This is wrong; only states that explicitly want error tokens
-     * should shift them.  */
-    yyn = yydefact[yystate];	/* If its default is to accept any token, ok.  Otherwise pop it. */
-    if (yyn)
-	goto yydefault;
-#endif
-
   yyerrpop:			/* pop the current state because it cannot handle the error token */
 
     if (yyssp == yyss)
