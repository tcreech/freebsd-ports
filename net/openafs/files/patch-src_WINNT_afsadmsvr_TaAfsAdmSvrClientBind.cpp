--- src/WINNT/afsadmsvr/TaAfsAdmSvrClientBind.cpp.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsadmsvr/TaAfsAdmSvrClientBind.cpp
@@ -57,26 +57,7 @@ BOOL ADMINAPI BindToAdminServer (LPCTSTR pszAddress, B
 
    for (DWORD dwTickStart = GetTickCount(); ; )
       {
-#ifdef notdef
-      // First we'll enumerate the name services around here to see if
-      // an admin server is already running.
-      //
-      RPC_NS_HANDLE hEnum;
-      if ((status = RpcNsBindingImportBegin (RPC_C_NS_SYNTAX_DEFAULT, pszENTRYNAME, ITaAfsAdminSvr_v1_0_c_ifspec, NULL, &hEnum)) == 0)
-         {
-         RPC_BINDING_HANDLE hBind;
-         status = RpcNsBindingImportNext (hEnum, &hBind);
-         RpcNsBindingImportDone (&hEnum);
-
-         if (status)
-            RpcBindingFree (&hBind);
-         else if (ValidateBinding (hBind, pidClient, (ULONG*)&status))
-            return TRUE;
-         else if (status != RPC_S_CALL_FAILED_DNE) // server rejected us!
-            break;
-         }
-#endif
-      // Failing that, we'll try to bind to the well-known endpoint that the
+      // Try to bind to the well-known endpoint that the
       // admin server may have had to use. (if RpcNsBindingExport failed.)
       //
       unsigned char *pszStringBinding = NULL;
