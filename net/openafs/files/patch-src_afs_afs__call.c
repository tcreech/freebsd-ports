--- src/afs/afs_call.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/afs_call.c
@@ -98,11 +98,19 @@ extern afs_int32 afs_md5inum;
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
@@ -127,10 +135,12 @@ afs_InitSetup(int preallocs)
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
@@ -139,6 +149,9 @@ afs_InitSetup(int preallocs)
     afs_InitSetup_done = 1;
     afs_osi_Wakeup(&afs_InitSetup_done);
 
+ done:
+    afs_InitSetup_running = 0;
+    afs_osi_Wakeup(&afs_InitSetup_running);
     return code;
 }
 
@@ -1484,9 +1497,12 @@ afs_syscall_call(long parm, long parm2, long parm3,
 	mtu = ((i == -1) ? htonl(1500) : afs_cb_interface.mtu[i]);
 # else /* AFS_USERSPACE_IP_ADDR */
 	rx_ifnet_t tifnp;
+	RX_NET_EPOCH_ENTER();
 
 	tifnp = rxi_FindIfnet(parm2, NULL);	/*  make iterative */
 	mtu = (tifnp ? rx_ifnet_mtu(tifnp) : htonl(1500));
+
+	RX_NET_EPOCH_EXIT();
 # endif /* else AFS_USERSPACE_IP_ADDR */
 #endif /* !AFS_SUN5_ENV */
 	if (!code)
@@ -1505,10 +1521,13 @@ afs_syscall_call(long parm, long parm2, long parm3,
 	}
 # else /* AFS_USERSPACE_IP_ADDR */
 	rx_ifnet_t tifnp;
+	RX_NET_EPOCH_ENTER();
 
 	tifnp = rxi_FindIfnet(parm2, &mask);	/* make iterative */
 	if (!tifnp)
 	    code = -1;
+
+	RX_NET_EPOCH_EXIT();
 # endif /* else AFS_USERSPACE_IP_ADDR */
 #endif /* !AFS_SUN5_ENV */
 	if (!code)
@@ -1691,7 +1710,9 @@ afs_shutdown(void)
     afs_warn("CB... ");
 
     afs_termState = AFSOP_STOP_RXCALLBACK;
+    AFS_GUNLOCK();
     rx_WakeupServerProcs();
+    AFS_GLOCK();
 #ifdef AFS_AIX51_ENV
     shutdown_rxkernel();
 #endif
@@ -1744,7 +1765,9 @@ afs_shutdown(void)
     afs_warn("NetIfPoller... ");
     osi_StopNetIfPoller();
 #endif
+    AFS_GUNLOCK();
     rxi_FreeAllPackets();
+    AFS_GLOCK();
 
     afs_termState = AFSOP_STOP_COMPLETE;
 
