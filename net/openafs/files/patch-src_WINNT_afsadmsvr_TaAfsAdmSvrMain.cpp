--- src/WINNT/afsadmsvr/TaAfsAdmSvrMain.cpp.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsadmsvr/TaAfsAdmSvrMain.cpp
@@ -85,9 +85,6 @@ int cdecl main (int argc, char **argv)
    // Clean up any broken interface registration
    //
    RpcServerUnregisterIf (ITaAfsAdminSvr_v1_0_s_ifspec, 0, FALSE);
-#ifdef notdef
-   RpcNsBindingUnexport (RPC_C_NS_SYNTAX_DEFAULT, pszENTRYNAME, ITaAfsAdminSvr_v1_0_s_ifspec, NULL);
-#endif
 
    // Register our interface
    //
@@ -125,13 +122,6 @@ int cdecl main (int argc, char **argv)
          {
          BOOL fExportedBinding = FALSE;
 
-#ifdef notdef
-         if ((status = RpcNsBindingExport (RPC_C_NS_SYNTAX_DEFAULT, pszENTRYNAME, ITaAfsAdminSvr_v1_0_s_ifspec, pBindingVector, NULL)) == 0)
-            fExportedBinding = TRUE;
-         else
-            Print (dlWARNING, TEXT("RpcNsBindingExport failed (benign); error 0x%08lX"), status);
-#endif
-
          if (!fExportedBinding && !fGotPort)
             {
             Print (dlERROR, TEXT("RpcNsBindingExport failed; error 0x%08lX"), status);
@@ -166,15 +156,6 @@ int cdecl main (int argc, char **argv)
             AfsAdmSvr_Shutdown();
             }
 
-#ifdef notdef
-         if (fExportedBinding)
-            {
-            if ((status = RpcNsBindingUnexport (RPC_C_NS_SYNTAX_DEFAULT, pszENTRYNAME, ITaAfsAdminSvr_v1_0_s_ifspec, NULL)) != 0)
-               {
-               Print (dlWARNING, TEXT("RpcNsBindingExport failed; error 0x%08lX"), status);
-               }
-            }
-#endif
          if ((status = RpcEpUnregister (ITaAfsAdminSvr_v1_0_s_ifspec, pBindingVector, NULL)) != 0)
             {
             Print (dlWARNING, TEXT("RpcEpUnregister failed; error 0x%08lX"), status);
