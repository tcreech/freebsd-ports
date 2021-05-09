--- src/rx/rx_packet.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_packet.c
@@ -31,7 +31,7 @@
 #  endif
 #  include "h/socket.h"
 #  if !defined(AFS_SUN5_ENV) &&  !defined(AFS_LINUX20_ENV) && !defined(AFS_HPUX110_ENV)
-#   if	!defined(AFS_OSF_ENV) && !defined(AFS_AIX41_ENV)
+#   if	!defined(AFS_AIX41_ENV)
 #    include "sys/mount.h"		/* it gets pulled in by something later anyway */
 #   endif
 #   include "h/mbuf.h"
@@ -1895,7 +1895,7 @@ rxi_ReceiveDebugPacket(struct rx_packet *ap, osi_socke
 
 			tconn.natMTU = htonl(tc->peer->natMTU);
 			tconn.error = htonl(tc->error);
-			tconn.flags = tc->flags;
+			tconn.flags = (u_char) (tc->flags & 0xff);  /* compat. */
 			tconn.type = tc->type;
 			tconn.securityIndex = tc->securityIndex;
 			if (tc->securityObject) {
@@ -2149,7 +2149,7 @@ rxi_SendDebugPacket(struct rx_packet *apacket, osi_soc
 	if (!waslocked)
 	    AFS_GLOCK();
 	afs_Trace1(afs_iclSetp, CM_TRACE_TIMESTAMP, ICL_TYPE_STRING,
-		   "before osi_NetSend()");
+		   "before rxi_NetSend()");
 	AFS_GUNLOCK();
     }
 #else
@@ -2158,14 +2158,14 @@ rxi_SendDebugPacket(struct rx_packet *apacket, osi_soc
 #endif
 #endif
     /* debug packets are not reliably delivered, hence the cast below. */
-    (void)osi_NetSend(asocket, &taddr, apacket->wirevec, apacket->niovecs,
+    (void)rxi_NetSend(asocket, &taddr, apacket->wirevec, apacket->niovecs,
 		      apacket->length + RX_HEADER_SIZE, istack);
 #ifdef KERNEL
 #ifdef RX_KERNEL_TRACE
     if (ICL_SETACTIVE(afs_iclSetp)) {
 	AFS_GLOCK();
 	afs_Trace1(afs_iclSetp, CM_TRACE_TIMESTAMP, ICL_TYPE_STRING,
-		   "after osi_NetSend()");
+		   "after rxi_NetSend()");
 	if (!waslocked)
 	    AFS_GUNLOCK();
     }
@@ -2302,7 +2302,7 @@ rxi_SendPacket(struct rx_call *call, struct rx_connect
 	    if (!waslocked)
 		AFS_GLOCK();
 	    afs_Trace1(afs_iclSetp, CM_TRACE_TIMESTAMP, ICL_TYPE_STRING,
-		       "before osi_NetSend()");
+		       "before rxi_NetSend()");
 	    AFS_GUNLOCK();
 	}
 #else
@@ -2311,7 +2311,7 @@ rxi_SendPacket(struct rx_call *call, struct rx_connect
 #endif
 #endif
 	if ((code =
-	     osi_NetSend(socket, &addr, p->wirevec, p->niovecs,
+	     rxi_NetSend(socket, &addr, p->wirevec, p->niovecs,
 			 p->length + RX_HEADER_SIZE, istack)) != 0) {
 	    /* send failed, so let's hurry up the resend, eh? */
             if (rx_stats_active)
@@ -2332,7 +2332,7 @@ rxi_SendPacket(struct rx_call *call, struct rx_connect
 	if (ICL_SETACTIVE(afs_iclSetp)) {
 	    AFS_GLOCK();
 	    afs_Trace1(afs_iclSetp, CM_TRACE_TIMESTAMP, ICL_TYPE_STRING,
-		       "after osi_NetSend()");
+		       "after rxi_NetSend()");
 	    if (!waslocked)
 		AFS_GUNLOCK();
 	}
@@ -2509,7 +2509,7 @@ rxi_SendPacketList(struct rx_call *call, struct rx_con
 	    AFS_GUNLOCK();
 #endif
 	if ((code =
-	     osi_NetSend(socket, &addr, &wirevec[0], len + 1, length,
+	     rxi_NetSend(socket, &addr, &wirevec[0], len + 1, length,
 			 istack)) != 0) {
 	    /* send failed, so let's hurry up the resend, eh? */
             if (rx_stats_active)
@@ -2592,7 +2592,7 @@ rxi_SendRawAbort(osi_socket socket, afs_uint32 host, u
     addr.sin_len = sizeof(struct sockaddr_in);
 #endif
 
-    osi_NetSend(socket, &addr, iov, 2,
+    rxi_NetSend(socket, &addr, iov, 2,
 		sizeof(struct rx_header) + sizeof(error), istack);
 }
 
