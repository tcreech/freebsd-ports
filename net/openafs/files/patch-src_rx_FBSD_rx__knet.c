--- src/rx/FBSD/rx_knet.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/FBSD/rx_knet.c
@@ -69,7 +69,7 @@ osi_NetReceive(osi_socket asocket, struct sockaddr_in 
 		*addr = *(struct sockaddr_in *)sa;
 	} else
 	    printf("Unknown socket family %d in NetReceive\n", sa->sa_family);
-	FREE(sa, M_SONAME);
+	free(sa, M_SONAME);
     }
     return code;
 }
@@ -96,11 +96,7 @@ osi_StopListener(void)
     p = pfind(rxk_ListenerPid);
     if (p) {
 	afs_warn("osi_StopListener: rxk_ListenerPid %u\n", rxk_ListenerPid);
-#if (__FreeBSD_version >= 900044)
 	kern_psignal(p, SIGUSR1);
-#else
-	psignal(p, SIGUSR1);
-#endif
 	PROC_UNLOCK(p);
     } else
 	afs_warn("osi_StopListener: rxk_Listener not found (pid %u)\n",
@@ -197,11 +193,6 @@ rxk_init()
     last = inetdomain.dom_protoswNPROTOSW;
     for (tpro = inetdomain.dom_protosw; tpro < last; tpro++)
 	if (tpro->pr_protocol == IPPROTO_UDP) {
-#if 0				/* not exported */
-	    /* force UDP checksumming on for AFS    */
-	    extern int udpcksum;
-	    udpcksum = 1;
-#endif
 	    memcpy(&parent_proto, tpro, sizeof(parent_proto));
 	    tpro->pr_input = rxk_input;
 	    tpro->pr_fasttimo = rxk_fasttimo;
