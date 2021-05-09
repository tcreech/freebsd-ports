--- src/WINNT/afsd/rpc_srvsvc.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/rpc_srvsvc.c
@@ -1426,18 +1426,6 @@ void __RPC_USER SHARE_DEL_HANDLE_rundown( SHARE_DEL_HA
 {
 }
 
-#if 0
-/* [nocode] */ void Opnum3NotUsedOnWire(
-    /* [in] */ handle_t IDL_handle)
-{
-}
-
-/* [nocode] */ void Opnum4NotUsedOnWire(
-    /* [in] */ handle_t IDL_handle)
-{
-}
-#endif
-
 /* [nocode] */ void Opnum5NotUsedOnWire(
     /* [in] */ handle_t IDL_handle)
 {
