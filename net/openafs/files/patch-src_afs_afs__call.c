--- src/afs/afs_call.c.orig	2024-10-03 22:32:45 UTC
+++ src/afs/afs_call.c
@@ -105,11 +105,19 @@ afs_InitSetup(int preallocs)
 static int
 afs_InitSetup(int preallocs)
 {
+    static int afs_InitSetup_running;
+
     int code;
 
+    while (afs_InitSetup_running) {
+	afs_osi_Sleep(&afs_InitSetup_running);
+    }
+
     if (afs_InitSetup_done)
 	return EAGAIN;
 
+    afs_InitSetup_running = 1;
+
 #ifdef AFS_SUN510_ENV
     /* Initialize a RW lock for the ifinfo global array */
     rw_init(&afsifinfo_lock, NULL, RW_DRIVER, NULL);
@@ -134,10 +142,12 @@ afs_InitSetup(int preallocs)
     /* start RX */
     if(!afscall_set_rxpck_received)
     rx_extraPackets = AFS_NRXPACKETS;	/* smaller # of packets */
+    AFS_GUNLOCK();
     code = rx_InitHost(rx_bindhost, htons(7001));
+    AFS_GLOCK();
     if (code) {
 	afs_warn("AFS: RX failed to initialize %d).\n", code);
-	return code;
+	goto done;
     }
     rx_SetRxDeadTime(afs_rx_deadtime);
     /* resource init creates the services */
@@ -146,6 +156,9 @@ afs_InitSetup(int preallocs)
     afs_InitSetup_done = 1;
     afs_osi_Wakeup(&afs_InitSetup_done);
 
+ done:
+    afs_InitSetup_running = 0;
+    afs_osi_Wakeup(&afs_InitSetup_running);
     return code;
 }
 
@@ -1704,7 +1717,9 @@ afs_shutdown(enum afs_shutdown_type cold_flag)
     afs_warn("CB... ");
 
     afs_termState = AFSOP_STOP_RXCALLBACK;
+    AFS_GUNLOCK();
     rx_WakeupServerProcs();
+    AFS_GLOCK();
 #ifdef AFS_AIX51_ENV
     shutdown_rxkernel();
 #endif
@@ -1757,7 +1772,9 @@ afs_shutdown(enum afs_shutdown_type cold_flag)
     afs_warn("NetIfPoller... ");
     osi_StopNetIfPoller();
 #endif
+    AFS_GUNLOCK();
     rxi_FreeAllPackets();
+    AFS_GLOCK();
 
     afs_termState = AFSOP_STOP_COMPLETE;
 
