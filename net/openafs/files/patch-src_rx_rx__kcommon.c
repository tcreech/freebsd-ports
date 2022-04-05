--- src/rx/rx_kcommon.c.orig	2021-12-09 17:07:41 UTC
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
@@ -684,7 +678,9 @@ rxi_GetIFInfo(void)
 	    if (i >= ADDRSPERSITE)
 		break;
 #   elif defined(AFS_FBSD_ENV)
+#    if !defined(AFS_FBSD_EXPLICIT_EPOCH_TRACKER)
 	if_addr_rlock(ifn);
+#    endif
 	AFS_FBSD_NET_FOREACH(ifad, &ifn->if_addrhead, ifa_link) {
 	    if (i >= ADDRSPERSITE)
 		break;
@@ -718,7 +714,9 @@ rxi_GetIFInfo(void)
 	    }
 	}
 #   ifdef AFS_FBSD_ENV
+#    if !defined(AFS_FBSD_EXPLICIT_EPOCH_TRACKER)
 	if_addr_runlock(ifn);
+#    endif
 #   endif
     }
 #  endif /* !AFS_DARWIN80_ENV */
@@ -857,8 +855,6 @@ rxk_NewSocketHost(afs_uint32 ahost, short aport)
 #  if (defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)) && defined(KERNEL_FUNNEL)
     thread_funnel_switch(KERNEL_FUNNEL, NETWORK_FUNNEL);
 #  endif
-    AFS_ASSERT_GLOCK();
-    AFS_GUNLOCK();
 #  if	defined(AFS_HPUX102_ENV)
 #   if     defined(AFS_HPUX110_ENV)
     /* we need a file associated with the socket so sosend in NetSend
@@ -998,14 +994,12 @@ rxk_NewSocketHost(afs_uint32 ahost, short aport)
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
