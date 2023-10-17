--- src/rx/FBSD/rx_knet.c.orig	2018-04-17 04:47:46 UTC
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
