--- src/rx/rx_kcommon.c.orig	2021-07-29 10:24:31 UTC
+++ src/rx/rx_kcommon.c
@@ -268,13 +268,7 @@ rx_ServerProc(void *unused)
     threadID = rxi_availProcs++;
     MUTEX_EXIT(&rx_quota_mutex);
 
-# ifdef RX_ENABLE_LOCKS
-    AFS_GUNLOCK();
-# endif /* RX_ENABLE_LOCKS */
     rxi_ServerProc(threadID, NULL, NULL);
-# ifdef RX_ENABLE_LOCKS
-    AFS_GLOCK();
-# endif /* RX_ENABLE_LOCKS */
 
     return NULL;
 }
@@ -389,6 +383,8 @@ rxi_InitPeerParams(struct rx_peer *pp)
 # else /* AFS_USERSPACE_IP_ADDR */
     rx_ifnet_t ifn;
 
+    RX_NET_EPOCH_ENTER();
+
 #  if !defined(AFS_SGI62_ENV)
     if (numMyNetAddrs == 0)
 	(void)rxi_GetIFInfo();
@@ -416,6 +412,9 @@ rxi_InitPeerParams(struct rx_peer *pp)
 	rx_rto_setPeerTimeoutSecs(pp, 3);
 	pp->ifMTU = MIN(RX_REMOTE_PACKET_SIZE, rx_MyMaxSendSize);
     }
+
+    RX_NET_EPOCH_EXIT();
+
 # endif /* else AFS_USERSPACE_IP_ADDR */
 #else /* AFS_SUN5_ENV */
     afs_int32 mtu;
@@ -657,15 +656,16 @@ rxi_GetIFInfo(void)
 	ifnet_list_free(ifns);
     }
 #  else /* AFS_DARWIN80_ENV */
-#   if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
-#    if defined(AFS_FBSD_ENV)
-    CURVNET_SET(rx_socket->so_vnet);
-    TAILQ_FOREACH(ifn, &V_ifnet, if_link) {
-#    else
+#   if defined(AFS_DARWIN_ENV)
     TAILQ_FOREACH(ifn, &ifnet, if_link) {
-#    endif
 	if (i >= ADDRSPERSITE)
 	    break;
+#   elif defined(AFS_FBSD_ENV)
+    CURVNET_SET(rx_socket->so_vnet);
+    IFNET_RLOCK();
+    AFS_FBSD_NET_FOREACH(ifn, &V_ifnet, if_link) {
+	if (i >= ADDRSPERSITE)
+	    break;
 #   elif defined(AFS_OBSD_ENV) || defined(AFS_NBSD_ENV)
     for (ifn = ifnet.tqh_first; i < ADDRSPERSITE && ifn != NULL;
 	 ifn = ifn->if_list.tqe_next) {
@@ -673,10 +673,17 @@ rxi_GetIFInfo(void)
     for (ifn = ifnet; ifn != NULL && i < ADDRSPERSITE; ifn = ifn->if_next) {
 #   endif
 	rxmtu = (ifn->if_mtu - RX_IPUDP_SIZE);
-#   if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
+#   if defined(AFS_DARWIN_ENV)
 	TAILQ_FOREACH(ifad, &ifn->if_addrhead, ifa_link) {
 	    if (i >= ADDRSPERSITE)
 		break;
+#   elif defined(AFS_FBSD_ENV)
+#    if !defined(AFS_FBSD_EXPLICIT_EPOCH_TRACKER)
+	if_addr_rlock(ifn);
+#    endif
+	AFS_FBSD_NET_FOREACH(ifad, &ifn->if_addrhead, ifa_link) {
+	    if (i >= ADDRSPERSITE)
+		break;
 #   elif defined(AFS_OBSD_ENV) || defined(AFS_NBSD_ENV)
 	for (ifad = ifn->if_addrlist.tqh_first;
 	     ifad != NULL && i < ADDRSPERSITE;
@@ -706,6 +713,11 @@ rxi_GetIFInfo(void)
 		}
 	    }
 	}
+#   ifdef AFS_FBSD_ENV
+#    if !defined(AFS_FBSD_EXPLICIT_EPOCH_TRACKER)
+	if_addr_runlock(ifn);
+#    endif
+#   endif
     }
 #  endif /* !AFS_DARWIN80_ENV */
 
@@ -723,6 +735,7 @@ rxi_GetIFInfo(void)
     }
 
 #  ifdef AFS_FBSD_ENV
+    IFNET_RUNLOCK();
     CURVNET_RESTORE();
 #  endif
 
@@ -842,8 +855,6 @@ rxk_NewSocketHost(afs_uint32 ahost, short aport)
 #  if (defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)) && defined(KERNEL_FUNNEL)
     thread_funnel_switch(KERNEL_FUNNEL, NETWORK_FUNNEL);
 #  endif
-    AFS_ASSERT_GLOCK();
-    AFS_GUNLOCK();
 #  if	defined(AFS_HPUX102_ENV)
 #   if     defined(AFS_HPUX110_ENV)
     /* we need a file associated with the socket so sosend in NetSend
@@ -983,14 +994,12 @@ rxk_NewSocketHost(afs_uint32 ahost, short aport)
 #   endif /* else defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV) */
 #  endif /* else AFS_HPUX110_ENV */
 
-    AFS_GLOCK();
 #  if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
     thread_funnel_switch(NETWORK_FUNNEL, KERNEL_FUNNEL);
 #  endif
     return (osi_socket *)newSocket;
 
   bad:
-    AFS_GLOCK();
 #  if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
     thread_funnel_switch(NETWORK_FUNNEL, KERNEL_FUNNEL);
 #  endif
