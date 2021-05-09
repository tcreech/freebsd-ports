--- tests/rpctestlib/rpc_test_procs.c.orig	2021-01-14 21:08:41 UTC
+++ tests/rpctestlib/rpc_test_procs.c
@@ -179,12 +179,7 @@ afs_int32 init_callback_service(rpc_test_request_ctx *
     afs_uuid_create(&(ctx->cb_listen_addr.uuid));
 
 #if !defined(RPC_TEST_GLOBAL_RX_INIT)
-#if 0
-    code = rx_InitHost(ctx->cb_listen_addr.addr_in[0],
-                       (int) htons(ctx->cb_port));
-#else
     code = rx_Init((int) htons(ctx->cb_port));
-#endif
 #endif /* RPC_TEST_GLOBAL_RX_INIT */
 
     assert(pthread_attr_init(&tattr) == 0);
