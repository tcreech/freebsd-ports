--- src/afs/afs_pag_call.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/afs_pag_call.c
@@ -91,10 +91,13 @@ afspag_Init(afs_int32 nfs_server_addr)
     afs_uuid_create(&afs_cb_interface.uuid);
 
     AFS_GLOCK();
-
     afs_InitStats();
+    AFS_GUNLOCK();
+
     rx_Init(htons(7001));
 
+    AFS_GLOCK();
+
     AFS_STATCNT(afs_ResourceInit);
     AFS_RWLOCK_INIT(&afs_xuser, "afs_xuser");
     AFS_RWLOCK_INIT(&afs_xpagcell, "afs_xpagcell");
@@ -117,18 +120,20 @@ afspag_Init(afs_int32 nfs_server_addr)
 			      1, RXSTATS_ExecuteRequest);
     pagcb_svc = rx_NewService(0, PAGCB_SERVICEID, "pagcb", &srv_secobj,
 			      1, PAGCB_ExecuteRequest);
+    AFS_GUNLOCK();
     rx_StartServer(0);
+    AFS_GLOCK();
 
     clt_secobj = rxnull_NewClientSecurityObject();
     rmtsys_conn = rx_NewConnection(nfs_server_addr, htons(7009),
 				   RMTSYS_SERVICEID, clt_secobj, 0);
 
 #ifdef RXK_LISTENER_ENV
-    afs_start_thread(rxk_Listener,       "Rx Listener");
+    afs_start_thread(rxk_Listener, "Rx Listener", 1);
 #endif
-    afs_start_thread((void *)(void *)rx_ServerProc,      "Rx Server Thread");
-    afs_start_thread(afs_rxevent_daemon, "Rx Event Daemon");
-    afs_start_thread(afs_Daemon,         "AFS PAG Daemon");
+    afs_start_thread((void *)(void *)rx_ServerProc, "Rx Server Thread", 0);
+    afs_start_thread(afs_rxevent_daemon, "Rx Event Daemon", 1);
+    afs_start_thread(afs_Daemon, "AFS PAG Daemon", 1);
 
     afs_icl_InitLogs();
 
