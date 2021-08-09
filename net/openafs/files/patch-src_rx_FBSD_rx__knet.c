--- src/rx/FBSD/rx_knet.c.orig	2021-07-29 10:24:31 UTC
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
