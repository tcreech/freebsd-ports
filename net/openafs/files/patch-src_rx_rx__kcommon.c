--- src/rx/rx_kcommon.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_kcommon.c
@@ -23,8 +23,8 @@
 #include "rx_peer.h"
 
 #ifdef AFS_HPUX110_ENV
-#include "h/tihdr.h"
-#include <xti.h>
+# include "h/tihdr.h"
+# include <xti.h>
 #endif
 #include "afsint.h"
 
@@ -42,15 +42,15 @@ rxk_portRocks_t rxk_portRocks;
 int rxk_initDone = 0;
 
 #if !defined(AFS_SUN5_ENV) && !defined(AFS_SGI62_ENV)
-#define ADDRSPERSITE 16
+# define ADDRSPERSITE 16
 static afs_uint32 myNetAddrs[ADDRSPERSITE];
 static int myNetMTUs[ADDRSPERSITE];
 static int numMyNetAddrs = 0;
 #endif
 
 #if defined(AFS_DARWIN80_ENV)
-#define sobind sock_bind
-#define soclose sock_close
+# define sobind sock_bind
+# define soclose sock_close
 #endif
 
 /* add a port to the monitored list, port # is in network order */
@@ -232,7 +232,7 @@ osi_AssertFailK(const char *expr, const char *file, in
 
     /* okay, panic */
 
-#define ADDBUF(BUF, STR)					\
+# define ADDBUF(BUF, STR)					\
 	if (strlen(BUF) + strlen((char *)(STR)) + 1 <= sizeof BUF) {	\
 		strcat(BUF, (char *)(STR));				\
 	}
@@ -246,11 +246,11 @@ osi_AssertFailK(const char *expr, const char *file, in
     ADDBUF(buf, linebuf);
     ADDBUF(buf, msg3);
 
-#undef ADDBUF
+# undef ADDBUF
 
     osi_Panic("%s", buf);
 }
-#endif
+#endif /* !AFS_LINUX26_ENV */
 
 #ifndef UKERNEL
 /* This is the server process request loop. Kernel server
@@ -268,13 +268,7 @@ rx_ServerProc(void *unused)
     threadID = rxi_availProcs++;
     MUTEX_EXIT(&rx_quota_mutex);
 
-#ifdef RX_ENABLE_LOCKS
-    AFS_GUNLOCK();
-#endif /* RX_ENABLE_LOCKS */
     rxi_ServerProc(threadID, NULL, NULL);
-#ifdef RX_ENABLE_LOCKS
-    AFS_GLOCK();
-#endif /* RX_ENABLE_LOCKS */
 
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
@@ -486,7 +485,7 @@ shutdown_rxkernel(void)
 #if !defined(AFS_SUN5_ENV) && !defined(AFS_SGI62_ENV)
 /* Determine what the network interfaces are for this machine. */
 
-#ifdef AFS_USERSPACE_IP_ADDR
+# ifdef AFS_USERSPACE_IP_ADDR
 int
 rxi_GetcbiInfo(void)
 {
@@ -587,13 +586,13 @@ rxi_Findcbi(afs_uint32 addr)
     return (rvalue);
 }
 
-#else /* AFS_USERSPACE_IP_ADDR */
+# else /* AFS_USERSPACE_IP_ADDR */
 
-#if !defined(AFS_AIX41_ENV) && !defined(AFS_DUX40_ENV) && !defined(AFS_DARWIN_ENV) && !defined(AFS_XBSD_ENV)
-#define IFADDR2SA(f) (&((f)->ifa_addr))
-#else /* AFS_AIX41_ENV */
-#define IFADDR2SA(f) ((f)->ifa_addr)
-#endif
+#  if !defined(AFS_AIX41_ENV) && !defined(AFS_DARWIN_ENV) && !defined(AFS_XBSD_ENV)
+#   define IFADDR2SA(f) (&((f)->ifa_addr))
+#  else
+#   define IFADDR2SA(f) ((f)->ifa_addr)
+#  endif
 
 int
 rxi_GetIFInfo(void)
@@ -605,7 +604,7 @@ rxi_GetIFInfo(void)
     afs_uint32 addrs[ADDRSPERSITE];
     int mtus[ADDRSPERSITE];
     afs_uint32 ifinaddr;
-#if defined(AFS_DARWIN80_ENV)
+#  if defined(AFS_DARWIN80_ENV)
     errno_t t;
     unsigned int count;
     int cnt=0, m, j;
@@ -614,15 +613,15 @@ rxi_GetIFInfo(void)
     struct sockaddr sout;
     struct sockaddr_in *sin;
     struct in_addr pin;
-#else
+#  else
     rx_ifaddr_t ifad;	/* ifnet points to a if_addrlist of ifaddrs */
     rx_ifnet_t ifn;
-#endif
+#  endif
 
     memset(addrs, 0, sizeof(addrs));
     memset(mtus, 0, sizeof(mtus));
 
-#if defined(AFS_DARWIN80_ENV)
+#  if defined(AFS_DARWIN80_ENV)
     if (!ifnet_list_get(AF_INET, &ifns, &count)) {
 	for (m = 0; m < count; m++) {
 	    if (!ifnet_get_address_list(ifns[m], &ifads)) {
@@ -656,34 +655,43 @@ rxi_GetIFInfo(void)
 	}
 	ifnet_list_free(ifns);
     }
-#else
-#if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
-#if defined(AFS_FBSD80_ENV)
-    TAILQ_FOREACH(ifn, &V_ifnet, if_link) {
-#else
+#  else /* AFS_DARWIN80_ENV */
+#   if defined(AFS_DARWIN_ENV)
     TAILQ_FOREACH(ifn, &ifnet, if_link) {
-#endif
 	if (i >= ADDRSPERSITE)
 	    break;
-#elif defined(AFS_OBSD_ENV) || defined(AFS_NBSD_ENV)
+#   elif defined(AFS_FBSD_ENV)
+    CURVNET_SET(rx_socket->so_vnet);
+    IFNET_RLOCK();
+    AFS_FBSD_NET_FOREACH(ifn, &V_ifnet, if_link) {
+	if (i >= ADDRSPERSITE)
+	    break;
+#   elif defined(AFS_OBSD_ENV) || defined(AFS_NBSD_ENV)
     for (ifn = ifnet.tqh_first; i < ADDRSPERSITE && ifn != NULL;
 	 ifn = ifn->if_list.tqe_next) {
-#else
+#   else
     for (ifn = ifnet; ifn != NULL && i < ADDRSPERSITE; ifn = ifn->if_next) {
-#endif
+#   endif
 	rxmtu = (ifn->if_mtu - RX_IPUDP_SIZE);
-#if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
+#   if defined(AFS_DARWIN_ENV)
 	TAILQ_FOREACH(ifad, &ifn->if_addrhead, ifa_link) {
 	    if (i >= ADDRSPERSITE)
 		break;
-#elif defined(AFS_OBSD_ENV) || defined(AFS_NBSD_ENV)
+#   elif defined(AFS_FBSD_ENV)
+#    if !defined(AFS_FBSD_EXPLICIT_EPOCH_TRACKER)
+	if_addr_rlock(ifn);
+#    endif
+	AFS_FBSD_NET_FOREACH(ifad, &ifn->if_addrhead, ifa_link) {
+	    if (i >= ADDRSPERSITE)
+		break;
+#   elif defined(AFS_OBSD_ENV) || defined(AFS_NBSD_ENV)
 	for (ifad = ifn->if_addrlist.tqh_first;
 	     ifad != NULL && i < ADDRSPERSITE;
 	     ifad = ifad->ifa_list.tqe_next) {
-#else
+#   else
 	for (ifad = ifn->if_addrlist; ifad != NULL && i < ADDRSPERSITE;
 	     ifad = ifad->ifa_next) {
-#endif
+#   endif
 	    if (IFADDR2SA(ifad)->sa_family == AF_INET) {
 		ifinaddr =
 		    ntohl(((struct sockaddr_in *)IFADDR2SA(ifad))->sin_addr.
@@ -705,8 +713,13 @@ rxi_GetIFInfo(void)
 		}
 	    }
 	}
+#   ifdef AFS_FBSD_ENV
+#    if !defined(AFS_FBSD_EXPLICIT_EPOCH_TRACKER)
+	if_addr_runlock(ifn);
+#    endif
+#   endif
     }
-#endif
+#  endif /* !AFS_DARWIN80_ENV */
 
     rx_maxJumboRecvSize =
 	RX_HEADER_SIZE + rxi_nDgramPackets * RX_JUMBOBUFFERSIZE +
@@ -720,17 +733,28 @@ rxi_GetIFInfo(void)
 	    myNetAddrs[l] = addrs[l];
 	}
     }
+
+#  ifdef AFS_FBSD_ENV
+    IFNET_RUNLOCK();
+    CURVNET_RESTORE();
+#  endif
+
     return different;
 }
 
-#if defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
+#  if defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
 /* Returns ifnet which best matches address */
 rx_ifnet_t
 rxi_FindIfnet(afs_uint32 addr, afs_uint32 * maskp)
 {
     struct sockaddr_in s, sr;
     rx_ifaddr_t ifad;
+    rx_ifnet_t ret;
 
+#   ifdef AFS_FBSD_ENV
+    CURVNET_SET(rx_socket->so_vnet);
+#   endif
+
     s.sin_family = AF_INET;
     s.sin_addr.s_addr = addr;
     ifad = rx_ifaddr_withnet((struct sockaddr *)&s);
@@ -739,10 +763,17 @@ rxi_FindIfnet(afs_uint32 addr, afs_uint32 * maskp)
 	rx_ifaddr_netmask(ifad, (struct sockaddr *)&sr, sizeof(sr));
 	*maskp = sr.sin_addr.s_addr;
     }
-    return (ifad ? rx_ifaddr_ifnet(ifad) : NULL);
+
+    ret = (ifad ? rx_ifaddr_ifnet(ifad) : NULL);
+
+#   ifdef AFS_FBSD_ENV
+    CURVNET_RESTORE();
+#   endif
+
+    return ret;
 }
 
-#else /* DARWIN || XBSD */
+#  else /* DARWIN || XBSD */
 
 /* Returns ifnet which best matches address */
 rx_ifnet_t
@@ -780,8 +811,8 @@ rxi_FindIfnet(afs_uint32 addr, afs_uint32 * maskp)
 	*maskp = ifad->ia_subnetmask;
     return (ifad ? ifad->ia_ifp : NULL);
 }
-#endif /* else DARWIN || XBSD */
-#endif /* else AFS_USERSPACE_IP_ADDR */
+#  endif /* else DARWIN || XBSD */
+# endif /* else AFS_USERSPACE_IP_ADDR */
 #endif /* !SUN5 && !SGI62 */
 
 
@@ -791,7 +822,7 @@ rxi_FindIfnet(afs_uint32 addr, afs_uint32 * maskp)
  * most of it is simple to follow common code.
  */
 #if !defined(UKERNEL)
-#if !defined(AFS_SUN5_ENV) && !defined(AFS_LINUX20_ENV)
+# if !defined(AFS_SUN5_ENV) && !defined(AFS_LINUX20_ENV)
 /* rxk_NewSocket creates a new socket on the specified port. The port is
  * in network byte order.
  */
@@ -799,35 +830,33 @@ osi_socket *
 rxk_NewSocketHost(afs_uint32 ahost, short aport)
 {
     afs_int32 code;
-#ifdef AFS_DARWIN80_ENV
+#  ifdef AFS_DARWIN80_ENV
     socket_t newSocket;
-#else
+#  else
     struct socket *newSocket;
-#endif
-#if (!defined(AFS_HPUX1122_ENV) && !defined(AFS_FBSD_ENV))
+#  endif
+#  if (!defined(AFS_HPUX1122_ENV) && !defined(AFS_FBSD_ENV))
     struct mbuf *nam;
-#endif
+#  endif
     struct sockaddr_in myaddr;
-#ifdef AFS_HPUX110_ENV
+#  ifdef AFS_HPUX110_ENV
     /* prototype copied from kernel source file streams/str_proto.h */
     extern MBLKP allocb_wait(int, int);
     MBLKP bindnam;
     int addrsize = sizeof(struct sockaddr_in);
     struct file *fp;
     extern struct fileops socketops;
-#endif
-#ifdef AFS_SGI65_ENV
+#  endif
+#  ifdef AFS_SGI65_ENV
     bhv_desc_t bhv;
-#endif
+#  endif
 
     AFS_STATCNT(osi_NewSocket);
-#if (defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)) && defined(KERNEL_FUNNEL)
+#  if (defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)) && defined(KERNEL_FUNNEL)
     thread_funnel_switch(KERNEL_FUNNEL, NETWORK_FUNNEL);
-#endif
-    AFS_ASSERT_GLOCK();
-    AFS_GUNLOCK();
-#if	defined(AFS_HPUX102_ENV)
-#if     defined(AFS_HPUX110_ENV)
+#  endif
+#  if	defined(AFS_HPUX102_ENV)
+#   if     defined(AFS_HPUX110_ENV)
     /* we need a file associated with the socket so sosend in NetSend
      * will not fail */
     /* blocking socket */
@@ -842,27 +871,27 @@ rxk_NewSocketHost(afs_uint32 ahost, short aport)
     fp->f_data = (void *)newSocket;
     newSocket->so_fp = (void *)fp;
 
-#else /* AFS_HPUX110_ENV */
+#   else /* AFS_HPUX110_ENV */
     code = socreate(AF_INET, &newSocket, SOCK_DGRAM, 0, SS_NOWAIT);
-#endif /* else AFS_HPUX110_ENV */
-#elif defined(AFS_SGI65_ENV) || defined(AFS_OBSD_ENV)
+#   endif /* else AFS_HPUX110_ENV */
+#  elif defined(AFS_SGI65_ENV) || defined(AFS_OBSD_ENV)
     code = socreate(AF_INET, &newSocket, SOCK_DGRAM, IPPROTO_UDP);
-#elif defined(AFS_FBSD_ENV)
+#  elif defined(AFS_FBSD_ENV)
     code = socreate(AF_INET, &newSocket, SOCK_DGRAM, IPPROTO_UDP,
 		    afs_osi_credp, curthread);
-#elif defined(AFS_DARWIN80_ENV)
-#ifdef RXK_LISTENER_ENV
+#  elif defined(AFS_DARWIN80_ENV)
+#   ifdef RXK_LISTENER_ENV
     code = sock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, NULL, &newSocket);
-#else
+#   else
     code = sock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, rx_upcall, NULL, &newSocket);
-#endif
-#elif defined(AFS_NBSD50_ENV)
+#   endif
+#  elif defined(AFS_NBSD50_ENV)
     code = socreate(AF_INET, &newSocket, SOCK_DGRAM, 0, osi_curproc(), NULL);
-#elif defined(AFS_NBSD40_ENV)
+#  elif defined(AFS_NBSD40_ENV)
     code = socreate(AF_INET, &newSocket, SOCK_DGRAM, 0, osi_curproc());
-#else
+#  else
     code = socreate(AF_INET, &newSocket, SOCK_DGRAM, 0);
-#endif /* AFS_HPUX102_ENV */
+#  endif /* AFS_HPUX102_ENV */
     if (code)
 	goto bad;
 
@@ -870,11 +899,11 @@ rxk_NewSocketHost(afs_uint32 ahost, short aport)
     myaddr.sin_family = AF_INET;
     myaddr.sin_port = aport;
     myaddr.sin_addr.s_addr = ahost;
-#ifdef STRUCT_SOCKADDR_HAS_SA_LEN
+#  ifdef STRUCT_SOCKADDR_HAS_SA_LEN
     myaddr.sin_len = sizeof(myaddr);
-#endif
+#  endif
 
-#ifdef AFS_HPUX110_ENV
+#  ifdef AFS_HPUX110_ENV
     bindnam = allocb_wait((addrsize + SO_MSGOFFSET + 1), BPRI_MED);
     if (!bindnam) {
 	setuerror(ENOBUFS);
@@ -886,15 +915,15 @@ rxk_NewSocketHost(afs_uint32 ahost, short aport)
     code = sobind(newSocket, bindnam, addrsize);
     if (code) {
 	soclose(newSocket);
-#if !defined(AFS_HPUX1122_ENV)
+#   if !defined(AFS_HPUX1122_ENV)
 	m_freem(nam);
-#endif
+#   endif
 	goto bad;
     }
 
     freeb(bindnam);
-#else /* AFS_HPUX110_ENV */
-#if defined(AFS_DARWIN80_ENV)
+#  else /* AFS_HPUX110_ENV */
+#   if defined(AFS_DARWIN80_ENV)
     {
        int buflen = 50000;
        int i,code2;
@@ -910,76 +939,70 @@ rxk_NewSocketHost(afs_uint32 ahost, short aport)
            buflen = 32766;
        }
     }
-#else
-#if defined(AFS_NBSD_ENV)
+#   else
+#    if defined(AFS_NBSD_ENV)
     solock(newSocket);
-#endif
+#    endif
     code = soreserve(newSocket, 50000, 50000);
     if (code) {
 	code = soreserve(newSocket, 32766, 32766);
 	if (code)
 	    osi_Panic("osi_NewSocket: last attempt to reserve 32K failed!\n");
     }
-#if defined(AFS_NBSD_ENV)
+#    if defined(AFS_NBSD_ENV)
     sounlock(newSocket);
-#endif
-#endif
-#if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
-#if defined(AFS_FBSD_ENV)
+#    endif
+#   endif
+#   if defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV)
+#    if defined(AFS_FBSD_ENV)
     code = sobind(newSocket, (struct sockaddr *)&myaddr, curthread);
-#else
+#    else
     code = sobind(newSocket, (struct sockaddr *)&myaddr);
-#endif
+#    endif
     if (code) {
 	dpf(("sobind fails (%d)\n", (int)code));
 	soclose(newSocket);
 	goto bad;
     }
-#else /* defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV) */
-#ifdef  AFS_OSF_ENV
-    nam = m_getclr(M_WAIT, MT_SONAME);
-#else /* AFS_OSF_ENV */
+#   else /* defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV) */
     nam = m_get(M_WAIT, MT_SONAME);
-#endif
     if (nam == NULL) {
-#if defined(KERNEL_HAVE_UERROR)
+#    if defined(KERNEL_HAVE_UERROR)
 	setuerror(ENOBUFS);
-#endif
+#    endif
 	goto bad;
     }
     nam->m_len = sizeof(myaddr);
     memcpy(mtod(nam, caddr_t), &myaddr, sizeof(myaddr));
-#if defined(AFS_SGI65_ENV)
+#    if defined(AFS_SGI65_ENV)
     BHV_PDATA(&bhv) = (void *)newSocket;
     code = sobind(&bhv, nam);
     m_freem(nam);
-#elif defined(AFS_OBSD44_ENV) || defined(AFS_NBSD40_ENV)
+#    elif defined(AFS_OBSD44_ENV) || defined(AFS_NBSD40_ENV)
     code = sobind(newSocket, nam, osi_curproc());
-#else
+#    else
     code = sobind(newSocket, nam);
-#endif
+#    endif
     if (code) {
 	dpf(("sobind fails (%d)\n", (int)code));
 	soclose(newSocket);
-#ifndef AFS_SGI65_ENV
+#    ifndef AFS_SGI65_ENV
 	m_freem(nam);
-#endif
+#    endif
 	goto bad;
     }
-#endif /* else AFS_DARWIN_ENV */
-#endif /* else AFS_HPUX110_ENV */
+#   endif /* else defined(AFS_DARWIN_ENV) || defined(AFS_FBSD_ENV) */
+#  endif /* else AFS_HPUX110_ENV */
 
-    AFS_GLOCK();
-#if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
+#  if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
     thread_funnel_switch(NETWORK_FUNNEL, KERNEL_FUNNEL);
-#endif
+#  endif
     return (osi_socket *)newSocket;
 
   bad:
-    AFS_GLOCK();
-#if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
+#  if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
     thread_funnel_switch(NETWORK_FUNNEL, KERNEL_FUNNEL);
-#endif
+#  endif
     return (osi_socket *)0;
 }
 
@@ -994,31 +1017,31 @@ int
 rxk_FreeSocket(struct socket *asocket)
 {
     AFS_STATCNT(osi_FreeSocket);
-#if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
+#  if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
     thread_funnel_switch(KERNEL_FUNNEL, NETWORK_FUNNEL);
-#endif
-#ifdef AFS_HPUX110_ENV
+#  endif
+#  ifdef AFS_HPUX110_ENV
     if (asocket->so_fp) {
 	struct file *fp = asocket->so_fp;
-#if !defined(AFS_HPUX1123_ENV)
+#   if !defined(AFS_HPUX1123_ENV)
 	/* 11.23 still has falloc, but not FPENTRYFREE !
 	 * so for now if we shutdown, we will waist a file
 	 * structure */
 	FPENTRYFREE(fp);
 	asocket->so_fp = NULL;
-#endif
+#   endif
     }
-#endif /* AFS_HPUX110_ENV */
+#  endif /* AFS_HPUX110_ENV */
     soclose(asocket);
-#if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
+#  if defined(AFS_DARWIN_ENV) && defined(KERNEL_FUNNEL)
     thread_funnel_switch(NETWORK_FUNNEL, KERNEL_FUNNEL);
-#endif
+#  endif
     return 0;
 }
-#endif /* !SUN5 && !LINUX20 */
+# endif /* !SUN5 && !LINUX20 */
 
-#if defined(RXK_LISTENER_ENV) || defined(AFS_SUN5_ENV) || defined(RXK_UPCALL_ENV)
-#ifdef RXK_TIMEDSLEEP_ENV
+# if defined(RXK_LISTENER_ENV) || defined(AFS_SUN5_ENV) || defined(RXK_UPCALL_ENV)
+#  ifdef RXK_TIMEDSLEEP_ENV
 /* Shutting down should wake us up, as should an earlier event. */
 void
 rxi_ReScheduleEvents(void)
@@ -1031,7 +1054,7 @@ rxi_ReScheduleEvents(void)
     if (!glock)
         AFS_GUNLOCK();
 }
-#endif
+#  endif
 /*
  * Run RX event daemon every second (5 times faster than rest of systems)
  */
@@ -1042,45 +1065,45 @@ afs_rxevent_daemon(void)
     SPLVAR;
 
     while (1) {
-#ifdef RX_ENABLE_LOCKS
+#  ifdef RX_ENABLE_LOCKS
 	AFS_GUNLOCK();
-#endif /* RX_ENABLE_LOCKS */
+#  endif /* RX_ENABLE_LOCKS */
 	NETPRI;
 	rxevent_RaiseEvents(&temp);
 	USERPRI;
-#ifdef RX_ENABLE_LOCKS
+#  ifdef RX_ENABLE_LOCKS
 	AFS_GLOCK();
-#endif /* RX_ENABLE_LOCKS */
-#ifdef RX_KERNEL_TRACE
+#  endif /* RX_ENABLE_LOCKS */
+#  ifdef RX_KERNEL_TRACE
 	afs_Trace1(afs_iclSetp, CM_TRACE_TIMESTAMP, ICL_TYPE_STRING,
 		   "before afs_osi_Wait()");
-#endif
-#ifdef RXK_TIMEDSLEEP_ENV
+#  endif
+#  ifdef RXK_TIMEDSLEEP_ENV
 	afs_osi_TimedSleep(&afs_termState, MAX(500, ((temp.sec * 1000) +
 						     (temp.usec / 1000))), 0);
-#else
+#  else
 	afs_osi_Wait(500, NULL, 0);
-#endif
-#ifdef RX_KERNEL_TRACE
+#  endif
+#  ifdef RX_KERNEL_TRACE
 	afs_Trace1(afs_iclSetp, CM_TRACE_TIMESTAMP, ICL_TYPE_STRING,
 		   "after afs_osi_Wait()");
-#endif
+#  endif
 	if (afs_termState == AFSOP_STOP_RXEVENT) {
-#ifdef RXK_LISTENER_ENV
+#  ifdef RXK_LISTENER_ENV
 	    afs_termState = AFSOP_STOP_RXK_LISTENER;
-#elif defined(AFS_SUN510_ENV) || defined(RXK_UPCALL_ENV)
+#  elif defined(AFS_SUN510_ENV) || defined(RXK_UPCALL_ENV)
 	    afs_termState = AFSOP_STOP_NETIF;
-#else
+#  else
 	    afs_termState = AFSOP_STOP_COMPLETE;
-#endif
+#  endif
 	    osi_rxWakeup(&afs_termState);
 	    return;
 	}
     }
 }
-#endif
+# endif /* RXK_LISTENER_ENV || AFS_SUN5_ENV || RXK_UPCALL_ENV */
 
-#ifdef RXK_LISTENER_ENV
+# ifdef RXK_LISTENER_ENV
 
 /* rxk_ReadPacket returns 1 if valid packet, 0 on error. */
 int
@@ -1115,24 +1138,24 @@ rxk_ReadPacket(osi_socket so, struct rx_packet *p, int
     p->wirevec[p->niovecs - 1].iov_len = savelen + RX_EXTRABUFFERSIZE;
 
     nbytes = tlen + sizeof(afs_int32);
-#ifdef RX_KERNEL_TRACE
+#  ifdef RX_KERNEL_TRACE
     if (ICL_SETACTIVE(afs_iclSetp)) {
 	AFS_GLOCK();
 	afs_Trace1(afs_iclSetp, CM_TRACE_TIMESTAMP, ICL_TYPE_STRING,
 		   "before osi_NetRecive()");
 	AFS_GUNLOCK();
     }
-#endif
+#  endif
     code = osi_NetReceive(rx_socket, &from, p->wirevec, p->niovecs, &nbytes);
 
-#ifdef RX_KERNEL_TRACE
+#  ifdef RX_KERNEL_TRACE
     if (ICL_SETACTIVE(afs_iclSetp)) {
 	AFS_GLOCK();
 	afs_Trace1(afs_iclSetp, CM_TRACE_TIMESTAMP, ICL_TYPE_STRING,
 		   "after osi_NetRecive()");
 	AFS_GUNLOCK();
     }
-#endif
+#  endif
     /* restore the vec to its correct state */
     p->wirevec[p->niovecs - 1].iov_len = savelen;
 
@@ -1162,10 +1185,10 @@ rxk_ReadPacket(osi_socket so, struct rx_packet *p, int
                 }
 	    }
 
-#ifdef RX_TRIMDATABUFS
+#  ifdef RX_TRIMDATABUFS
 	    /* Free any empty packet buffers at the end of this packet */
 	    rxi_TrimDataBufs(p, 1);
-#endif
+#  endif
 	    return 0;
 	}
     } else
@@ -1187,9 +1210,9 @@ rxk_ReadPacket(osi_socket so, struct rx_packet *p, int
  * OS's socket receive routine returns as a result of a signal.
  */
 int rxk_ListenerPid;		/* Used to signal process to wakeup at shutdown */
-#ifdef AFS_LINUX20_ENV
+#  ifdef AFS_LINUX20_ENV
 struct task_struct *rxk_ListenerTask;
-#endif
+#  endif
 
 void
 rxk_Listener(void)
@@ -1198,25 +1221,25 @@ rxk_Listener(void)
     int code;
     int host, port;
 
-#ifdef AFS_LINUX20_ENV
+#  ifdef AFS_LINUX20_ENV
     rxk_ListenerPid = current->pid;
     rxk_ListenerTask = current;
     allow_signal(SIGKILL);    /* Allowed, but blocked until shutdown */
-#endif
-#ifdef AFS_SUN5_ENV
+#  endif
+#  ifdef AFS_SUN5_ENV
     rxk_ListenerPid = 1;	/* No PID, just a flag that we're alive */
-#endif /* AFS_SUN5_ENV */
-#ifdef AFS_XBSD_ENV
+#  endif /* AFS_SUN5_ENV */
+#  ifdef AFS_XBSD_ENV
     rxk_ListenerPid = curproc->p_pid;
-#endif /* AFS_FBSD_ENV */
-#ifdef AFS_DARWIN80_ENV
+#  endif /* AFS_FBSD_ENV */
+#  ifdef AFS_DARWIN80_ENV
     rxk_ListenerPid = proc_selfpid();
-#elif defined(AFS_DARWIN_ENV)
+#  elif defined(AFS_DARWIN_ENV)
     rxk_ListenerPid = current_proc()->p_pid;
-#endif
-#ifdef RX_ENABLE_LOCKS
+#  endif
+#  ifdef RX_ENABLE_LOCKS
     AFS_GUNLOCK();
-#endif /* RX_ENABLE_LOCKS */
+#  endif /* RX_ENABLE_LOCKS */
     while (afs_termState != AFSOP_STOP_RXK_LISTENER) {
         /* See if a check for additional packets was issued */
         rx_CheckPackets();
@@ -1233,28 +1256,28 @@ rxk_Listener(void)
 	}
     }
 
-#ifdef RX_ENABLE_LOCKS
+#  ifdef RX_ENABLE_LOCKS
     AFS_GLOCK();
-#endif /* RX_ENABLE_LOCKS */
+#  endif /* RX_ENABLE_LOCKS */
     if (afs_termState == AFSOP_STOP_RXK_LISTENER) {
-#ifdef AFS_SUN510_ENV
+#  ifdef AFS_SUN510_ENV
 	afs_termState = AFSOP_STOP_NETIF;
-#else
+#  else
 	afs_termState = AFSOP_STOP_COMPLETE;
-#endif
+#  endif
 	osi_rxWakeup(&afs_termState);
     }
     rxk_ListenerPid = 0;
-#ifdef AFS_LINUX20_ENV
+#  ifdef AFS_LINUX20_ENV
     rxk_ListenerTask = 0;
     osi_rxWakeup(&rxk_ListenerTask);
-#endif
-#if defined(AFS_SUN5_ENV) || defined(AFS_FBSD_ENV)
+#  endif
+#  if defined(AFS_SUN5_ENV) || defined(AFS_FBSD_ENV)
     osi_rxWakeup(&rxk_ListenerPid);
-#endif
+#  endif
 }
 
-#if !defined(AFS_LINUX20_ENV) && !defined(AFS_SUN5_ENV) && !defined(AFS_DARWIN_ENV) && !defined(AFS_XBSD_ENV)
+#  if !defined(AFS_LINUX20_ENV) && !defined(AFS_SUN5_ENV) && !defined(AFS_DARWIN_ENV) && !defined(AFS_XBSD_ENV)
 /* The manner of stopping the rx listener thread may vary. Most unix's should
  * be able to call soclose.
  */
@@ -1263,19 +1286,19 @@ osi_StopListener(void)
 {
     soclose(rx_socket);
 }
-#endif
-#endif /* RXK_LISTENER_ENV */
-#endif /* !NCR && !UKERNEL */
+#  endif
+# endif /* RXK_LISTENER_ENV */
+#endif /* !UKERNEL */
 
 #if !defined(AFS_LINUX26_ENV)
 void
-#if defined(AFS_AIX_ENV)
+# if defined(AFS_AIX_ENV)
 osi_Panic(char *msg, void *a1, void *a2, void *a3)
-#else
+# else
 osi_Panic(char *msg, ...)
-#endif
+# endif
 {
-#ifdef AFS_AIX_ENV
+# ifdef AFS_AIX_ENV
     if (!msg)
 	msg = "Unknown AFS panic";
     /*
@@ -1285,7 +1308,7 @@ osi_Panic(char *msg, ...)
 
     printf(msg, a1, a2, a3);
     panic(msg);
-#elif defined(AFS_SGI_ENV)
+# elif defined(AFS_SGI_ENV)
     va_list ap;
 
     /* Solaris has vcmn_err, Sol10 01/06 may have issues. Beware. */
@@ -1296,7 +1319,7 @@ osi_Panic(char *msg, ...)
 	icmn_err(CE_PANIC, msg, ap);
 	va_end(ap);
     }
-#elif defined(AFS_DARWIN80_ENV) || defined(AFS_LINUX22_ENV) || defined(AFS_FBSD_ENV) || defined(UKERNEL)
+# elif defined(AFS_DARWIN80_ENV) || defined(AFS_LINUX22_ENV) || defined(AFS_FBSD_ENV) || defined(UKERNEL)
     char buf[256];
     va_list ap;
     if (!msg)
@@ -1307,7 +1330,7 @@ osi_Panic(char *msg, ...)
     va_end(ap);
     printf("%s", buf);
     panic("%s", buf);
-#else
+# else /* DARWIN80 || LINUX22 || FBSD || UKERNEL */
     va_list ap;
     if (!msg)
 	msg = "Unknown AFS panic";
@@ -1315,11 +1338,12 @@ osi_Panic(char *msg, ...)
     va_start(ap, msg);
     vprintf(msg, ap);
     va_end(ap);
-# ifdef AFS_LINUX20_ENV
+#  ifdef AFS_LINUX20_ENV
     * ((char *) 0) = 0;
-# else
+#  else
     panic("%s", msg);
-# endif
-#endif
+#  endif
+# endif /* else DARWIN80 || LINUX22 || FBSD || UKERNEL */
 }
-#endif
+
+#endif /* !AFS_LINUX26_ENV */
