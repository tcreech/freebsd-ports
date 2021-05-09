--- src/WINNT/client_osi/osidb.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/client_osi/osidb.c
@@ -107,36 +107,6 @@ void dbrpc_Close(handle_t handle, osi_remHyper_t *fnp,
 		*codep = OSI_DBRPC_NOFD;
 }
 
-#ifdef notdef
-long osi_CleanupRPCEntry(char *exportName)
-{
-	UUID_VECTOR uuidvp;
-	RPC_NS_HANDLE thandle;
-	UUID tuuid;
-	long code;
-
-	code = RpcNsEntryObjectInqBegin(RPC_C_NS_SYNTAX_DCE, exportName, &thandle);
-	if (code != RPC_S_OK && code != RPC_S_ENTRY_NOT_FOUND) return code;
-	while(1) {
-		code = RpcNsEntryObjectInqNext(thandle, &tuuid);
-		if (code == RPC_S_NO_MORE_MEMBERS) {
-			code = 0;
-			break;
-		}
-		else if (code != RPC_S_OK) {
-			break;
-		}
-		uuidvp.Count = 1;
-		uuidvp.Uuid[0] = &tuuid;
-		code = RpcNsBindingUnexport(RPC_C_NS_SYNTAX_DCE, exportName, dbrpc_v1_0_s_ifspec,
-			&uuidvp);
-		if (code != RPC_S_OK && code != RPC_S_INTERFACE_NOT_FOUND) break;
-	}
-	RpcNsEntryObjectInqDone(&thandle);
-	return code;
-}
-#endif /* notdef */
-
 long osi_InitDebug(osi_uid_t *exportIDp)
 {
 	RPC_STATUS rpcStatus;
@@ -174,21 +144,9 @@ long osi_InitDebug(osi_uid_t *exportIDp)
 	uuidVector.Count = 1;
 	uuidVector.Uuid[0] = exportIDp;
 
-#ifdef notdef
-	/* don't use CDS any longer; too big and slow */
-	rpcStatus = osi_CleanupRPCEntry(exportName);
-	if (rpcStatus) goto done;
-#endif /* notdef */
-
 	rpcStatus = RpcEpRegister(dbrpc_v1_0_s_ifspec, bindingVector,
 		&uuidVector, (unsigned char *) 0);
 	if (rpcStatus != RPC_S_OK) goto done;
-
-#ifdef notdef
-	/* don't use CDS */
-	rpcStatus = RpcNsBindingExport(RPC_C_NS_SYNTAX_DCE, exportName,
-		dbrpc_v1_0_s_ifspec, bindingVector, &uuidVector);
-#endif /* notdef */
 
 	rpcStatus = RpcBindingVectorFree(&bindingVector);
 	if (rpcStatus != RPC_S_OK) goto done;
