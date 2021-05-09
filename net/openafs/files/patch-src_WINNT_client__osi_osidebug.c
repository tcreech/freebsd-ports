--- src/WINNT/client_osi/osidebug.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/client_osi/osidebug.c
@@ -321,63 +321,6 @@ main_formatCache_t *main_GetFormatCache(RPC_BINDING_HA
 	else return NULL;
 }
 
-#ifdef notdef
-/* take a name and return a working binding to the server.  Server must be
- * exporting a dbrpc interface.
- */
-main_GetBinding(char *name, RPC_BINDING_HANDLE *handlep)
-{
-	RPC_NS_HANDLE tempContext;	/* context used for iteration */
-	RPC_BINDING_HANDLE thp;		/* temp handle we're using now */
-	long code;
-	int count=0;
-	int valid;
-
-	code = RpcNsBindingImportBegin(RPC_C_NS_SYNTAX_DCE, name, *dbrpc_v1_0_c_ifspecp,
-		(UUID *) 0, &tempContext);
-	if (code != RPC_S_OK) return code;
-
-	/* this next line is useful when trying to debug things */
-	// RpcNsMgmtHandleSetExpAge(tempContext, 0);
-
-	valid = 0;	/* is thp valid? */
-	while (1) {
-		code = RpcNsBindingImportNext(tempContext, &thp);
-		if (code == RPC_S_NO_MORE_BINDINGS) break;	/* we're done */
-
-		if (code != RPC_S_OK) {
-			RpcNsBindingImportDone(&tempContext);
-			return code;
-		}
-
-		/* otherwise we have a putatively good binding; try it out
-		 * with the probe interface and see if it really works
-		 */
-                code = RpcMgmtSetComTimeout(thp, 2);
-
-		code = 0;
-		RpcTryExcept {
-			dbrpc_Ping(thp);
-		}
-		RpcExcept(1) {
-			code = RpcExceptionCode();
-		}
-		RpcEndExcept;
-
-		/* now, if call worked, we're done */
-		if (code == 0) break;
-
-		/* else cleanup the binding and try again */
-		RpcBindingFree(&thp);
-	}
-
-	/* cleanup and return */
-	RpcNsBindingImportDone(&tempContext);
-	*handlep = thp;
-	return code;
-}
-#endif /* notdef */
-
 main_GetBinding(char *hostNamep, UUID *exportIDp, RPC_BINDING_HANDLE *handlep)
 {
 	char *stringBindingp;
