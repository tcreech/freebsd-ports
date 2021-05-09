--- src/afs/afs_server.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_server.c
@@ -37,36 +37,36 @@
 #include "afs/sysincludes.h"	/* Standard vendor system headers */
 
 #if !defined(UKERNEL)
-#if !defined(AFS_LINUX20_ENV)
-#include <net/if.h>
-#endif
-#include <netinet/in.h>
+# if !defined(AFS_LINUX20_ENV)
+#  include <net/if.h>
+# endif
+# include <netinet/in.h>
 
-#ifdef AFS_SGI62_ENV
-#include "h/hashing.h"
-#endif
-#if !defined(AFS_HPUX110_ENV) && !defined(AFS_LINUX20_ENV) && !defined(AFS_DARWIN_ENV)
-#include <netinet/in_var.h>
-#endif /* AFS_HPUX110_ENV */
-#ifdef AFS_DARWIN_ENV
-#include <net/if_var.h>
-#endif
+# ifdef AFS_SGI62_ENV
+#  include "h/hashing.h"
+# endif
+# if !defined(AFS_HPUX110_ENV) && !defined(AFS_LINUX20_ENV) && !defined(AFS_DARWIN_ENV)
+#  include <netinet/in_var.h>
+# endif /* AFS_HPUX110_ENV */
+# ifdef AFS_DARWIN_ENV
+#  include <net/if_var.h>
+# endif
 #endif /* !defined(UKERNEL) */
 
 #include "afsincludes.h"	/* Afs-based standard headers */
 #include "afs/afs_stats.h"	/* afs statistics */
 #include "rx/rx_multi.h"
 
-#if	defined(AFS_SUN5_ENV)
-#include <inet/led.h>
-#include <inet/common.h>
-#include <netinet/ip6.h>
-#define ipif_local_addr ipif_lcl_addr
-#ifndef V4_PART_OF_V6
-# define V4_PART_OF_V6(v6)       v6.s6_addr32[3]
+#if defined(AFS_SUN5_ENV)
+# include <inet/led.h>
+# include <inet/common.h>
+# include <netinet/ip6.h>
+# define ipif_local_addr ipif_lcl_addr
+# ifndef V4_PART_OF_V6
+#  define V4_PART_OF_V6(v6)       v6.s6_addr32[3]
+# endif
+# include <inet/ip.h>
 #endif
-#include <inet/ip.h>
-#endif
 
 /* Exported variables */
 afs_rwlock_t afs_xserver;	/* allocation lock for servers */
@@ -318,10 +318,10 @@ CheckVLServer(struct srvAddr *sa, struct vrequest *are
 
 
 #ifndef	AFS_MINCHANGE		/* So that some can increase it in param.h */
-#define AFS_MINCHANGE 2		/* min change we'll bother with */
+# define AFS_MINCHANGE 2		/* min change we'll bother with */
 #endif
 #ifndef AFS_MAXCHANGEBACK
-#define AFS_MAXCHANGEBACK 10	/* max seconds we'll set a clock back at once */
+# define AFS_MAXCHANGEBACK 10	/* max seconds we'll set a clock back at once */
 #endif
 
 
@@ -980,9 +980,9 @@ afs_SortServers(struct server *aservers[], int count)
 #define	USEIFADDR
 
 #ifdef AFS_USERSPACE_IP_ADDR
-#ifndef afs_min
-#define afs_min(A,B) ((A)<(B)) ? (A) : (B)
-#endif
+# ifndef afs_min
+#  define afs_min(A,B) ((A)<(B)) ? (A) : (B)
+# endif
 /*
  * The IP addresses and ranks are determined by afsd (in user space) and
  * passed into the kernel at startup time through the AFSOP_ADVISEADDR
@@ -1030,8 +1030,10 @@ afsi_SetServerIPRank(struct srvAddr *sa, afs_int32 add
     }
     return;
 }
+
 #else /* AFS_USERSPACE_IP_ADDR */
-#if (! defined(AFS_SUN5_ENV)) && (! defined(AFS_DARWIN_ENV)) && (! defined(AFS_OBSD47_ENV)) && (! defined(AFS_FBSD_ENV)) && defined(USEIFADDR)
+
+# if !defined(AFS_SUN5_ENV) && !defined(AFS_DARWIN_ENV) && !defined(AFS_OBSD47_ENV) && !defined(AFS_FBSD_ENV) && defined(USEIFADDR)
 void
 afsi_SetServerIPRank(struct srvAddr *sa, struct in_ifaddr *ifa)
 {
@@ -1054,7 +1056,7 @@ afsi_SetServerIPRank(struct srvAddr *sa, struct in_ifa
 		sa->sa_iprank = t;
 	}
     }
-#if defined(IFF_POINTOPOINT) && !defined(UKERNEL)
+#  if defined(IFF_POINTOPOINT) && !defined(UKERNEL)
     /* check for case #4 -- point-to-point link */
     if ((ifa->ia_ifp->if_flags & IFF_POINTOPOINT)
 	&& (SA2ULONG(IA_DST(ifa)) == ntohl(sa->sa_ip))) {
@@ -1065,23 +1067,23 @@ afsi_SetServerIPRank(struct srvAddr *sa, struct in_ifa
 	if (sa->sa_iprank > t)
 	    sa->sa_iprank = t;
     }
-#endif /* IFF_POINTOPOINT */
+#  endif /* IFF_POINTOPOINT */
 }
-#endif /*(!defined(AFS_SUN5_ENV)) && defined(USEIFADDR) */
-#if (defined(AFS_DARWIN_ENV) || defined(AFS_OBSD47_ENV) || defined(AFS_FBSD_ENV)) && defined(USEIFADDR)
-#ifndef afs_min
-#define afs_min(A,B) ((A)<(B)) ? (A) : (B)
-#endif
+# endif /* !SUN5 && !DARWIN && !OBSD47 && !FBSD && USEIFADDR */
+# if (defined(AFS_DARWIN_ENV) || defined(AFS_OBSD47_ENV) || defined(AFS_FBSD_ENV)) && defined(USEIFADDR)
+#  ifndef afs_min
+#   define afs_min(A,B) ((A)<(B)) ? (A) : (B)
+#  endif
 void
 afsi_SetServerIPRank(struct srvAddr *sa, rx_ifaddr_t ifa)
 {
     struct sockaddr sout;
     struct sockaddr_in *sin;
-#if defined(AFS_DARWIN80_ENV) && !defined(UKERNEL)
+#  if defined(AFS_DARWIN80_ENV) && !defined(UKERNEL)
     int t;
-#else
+#  else
     void *t;
-#endif
+#  endif
 
     afs_uint32 subnetmask, myAddr, myNet, myDstaddr, mySubnet, netMask;
     afs_uint32 serverAddr;
@@ -1134,7 +1136,7 @@ afsi_SetServerIPRank(struct srvAddr *sa, rx_ifaddr_t i
 	    sa->sa_iprank = afs_min(sa->sa_iprank, MED + rx_ifnet_metric(rx_ifaddr_ifnet(ifa)));
 	}
     }
-#ifdef  IFF_POINTTOPOINT
+#  ifdef  IFF_POINTTOPOINT
     /* check for case #4 -- point-to-point link */
     if ((rx_ifnet_flags(rx_ifaddr_ifnet(ifa)) & IFF_POINTOPOINT)
 	&& (myDstaddr == serverAddr)) {
@@ -1145,10 +1147,10 @@ afsi_SetServerIPRank(struct srvAddr *sa, rx_ifaddr_t i
 	if (sa->sa_iprank > t)
 	    sa->sa_iprank = t;
 	}
-#endif /* IFF_POINTTOPOINT */
+#  endif /* IFF_POINTTOPOINT */
 }
-#endif /*(!defined(AFS_SUN5_ENV)) && defined(USEIFADDR) */
-#endif /* else AFS_USERSPACE_IP_ADDR */
+# endif /* (DARWIN || OBSD47 || FBSD) && USEIFADDR */
+#endif /* AFS_USERSPACE_IP_ADDR */
 
 #ifdef AFS_SGI62_ENV
 static int
@@ -1162,7 +1164,7 @@ afsi_enum_set_rank(struct hashbucket *h, caddr_t mkey,
 static int
 afs_SetServerPrefs(struct srvAddr *const sa)
 {
-#if     defined(AFS_USERSPACE_IP_ADDR)
+#if defined(AFS_USERSPACE_IP_ADDR)
     int i;
 
       sa->sa_iprank = LO;
@@ -1171,19 +1173,19 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 			     afs_cb_interface.subnetmask[i]);
     }
 #else				/* AFS_USERSPACE_IP_ADDR */
-#if	defined(AFS_SUN5_ENV)
-#ifdef AFS_SUN510_ENV
+# if defined(AFS_SUN5_ENV)
+#  ifdef AFS_SUN510_ENV
     int i = 0;
-#else
+#  else
     extern struct ill_s *ill_g_headp;
     long *addr = (long *)ill_g_headp;
     ill_t *ill;
     ipif_t *ipif;
-#endif
+#  endif
     int subnet, subnetmask, net, netmask;
 
     sa->sa_iprank = 0;
-#ifdef AFS_SUN510_ENV
+#  ifdef AFS_SUN510_ENV
     rw_enter(&afsifinfo_lock, RW_READER);
 
     for (i = 0; (afsifinfo[i].ipaddr != NULL) && (i < ADDRSPERSITE); i++) {
@@ -1227,7 +1229,9 @@ afs_SetServerPrefs(struct srvAddr *const sa)
     }
 
     rw_exit(&afsifinfo_lock);
-#else
+
+#  else /* AFS_SUN510_ENV */
+
     for (ill = (struct ill_s *)*addr /*ill_g_headp */ ; ill;
 	 ill = ill->ill_next) {
 	/* Make sure this is an IPv4 ILL */
@@ -1277,9 +1281,9 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 	    }
 	}
     }
-#endif /* AFS_SUN510_ENV */
-#else
-#ifndef USEIFADDR
+#  endif /* AFS_SUN510_ENV */
+# else /* AFS_SUN5_ENV */
+#  ifndef USEIFADDR
     rx_ifnet_t ifn = NULL;
     struct in_ifaddr *ifad = (struct in_ifaddr *)0;
     struct sockaddr_in *sin;
@@ -1287,26 +1291,26 @@ afs_SetServerPrefs(struct srvAddr *const sa)
     sa->sa_iprank = 0;
     ifn = rxi_FindIfnet(sa->sa_ip, &ifad);
     if (ifn) {			/* local, more or less */
-#ifdef IFF_LOOPBACK
+#   ifdef IFF_LOOPBACK
 	if (ifn->if_flags & IFF_LOOPBACK) {
 	    sa->sa_iprank = TOPR;
 	    goto end;
 	}
-#endif /* IFF_LOOPBACK */
+#   endif /* IFF_LOOPBACK */
 	sin = (struct sockaddr_in *)IA_SIN(ifad);
 	if (SA2ULONG(sin) == sa->sa_ip) {
 	    sa->sa_iprank = TOPR;
 	    goto end;
 	}
-#ifdef IFF_BROADCAST
+#   ifdef IFF_BROADCAST
 	if (ifn->if_flags & IFF_BROADCAST) {
 	    if (sa->sa_ip == (sa->sa_ip & SA2ULONG(IA_BROAD(ifad)))) {
 		sa->sa_iprank = HI;
 		goto end;
 	    }
 	}
-#endif /* IFF_BROADCAST */
-#ifdef IFF_POINTOPOINT
+#   endif /* IFF_BROADCAST */
+#   ifdef IFF_POINTOPOINT
 	if (ifn->if_flags & IFF_POINTOPOINT) {
 	    if (sa->sa_ip == SA2ULONG(IA_DST(ifad))) {
 		if (ifn->if_metric > 4) {
@@ -1316,16 +1320,16 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 		    sa->sa_iprank = ifn->if_metric;
 	    }
 	}
-#endif /* IFF_POINTOPOINT */
+#   endif /* IFF_POINTOPOINT */
 	sa->sa_iprank += MED + ifn->if_metric;	/* couldn't find anything better */
     }
-#else				/* USEIFADDR */
+#  else /* USEIFADDR */
 
     sa->sa_iprank = LO;
-#ifdef AFS_SGI62_ENV
+#   ifdef AFS_SGI62_ENV
     (void)hash_enum(&hashinfo_inaddr, afsi_enum_set_rank, HTF_INET, NULL,
 		    (caddr_t) sa, NULL);
-#elif defined(AFS_DARWIN80_ENV)
+#   elif defined(AFS_DARWIN80_ENV)
     {
 	errno_t t;
 	unsigned int count;
@@ -1346,7 +1350,7 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 	    ifnet_list_free(ifns);
 	}
     }
-#elif defined(AFS_DARWIN_ENV)
+#   elif defined(AFS_DARWIN_ENV)
     {
 	rx_ifnet_t ifn;
 	rx_ifaddr_t ifa;
@@ -1354,43 +1358,45 @@ afs_SetServerPrefs(struct srvAddr *const sa)
 	    TAILQ_FOREACH(ifa, &ifn->if_addrhead, ifa_link) {
 		afsi_SetServerIPRank(sa, ifa);
     }}}
-#elif defined(AFS_FBSD_ENV)
+#   elif defined(AFS_FBSD_ENV)
     {
 	struct in_ifaddr *ifa;
-#if defined(AFS_FBSD80_ENV)
-	  TAILQ_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
-#else
-	  TAILQ_FOREACH(ifa, &in_ifaddrhead, ia_link) {
-#endif
+	struct rm_priotracker in_ifa_tracker;
+	CURVNET_SET(rx_socket->so_vnet);
+	IN_IFADDR_RLOCK(&in_ifa_tracker);
+	AFS_FBSD_NET_FOREACH(ifa, &V_in_ifaddrhead, ia_link) {
 	    afsi_SetServerIPRank(sa, &ifa->ia_ifa);
-    }}
-#elif defined(AFS_OBSD_ENV)
+	}
+	IN_IFADDR_RUNLOCK(&in_ifa_tracker);
+	CURVNET_RESTORE();
+    }
+#   elif defined(AFS_OBSD_ENV)
     {
 	extern struct in_ifaddrhead in_ifaddr;
 	struct in_ifaddr *ifa;
 	for (ifa = in_ifaddr.tqh_first; ifa; ifa = ifa->ia_list.tqe_next)
 	    afsi_SetServerIPRank(sa, ifa);
     }
-#elif defined(AFS_NBSD40_ENV)
+#   elif defined(AFS_NBSD40_ENV)
      {
        extern struct in_ifaddrhead in_ifaddrhead;
        struct in_ifaddr *ifa;
        for (ifa = in_ifaddrhead.tqh_first; ifa; ifa = ifa->ia_list.tqe_next)
            afsi_SetServerIPRank(sa, ifa);
      }
-#else
+#   else
     {
 	struct in_ifaddr *ifa;
 	for (ifa = in_ifaddr; ifa; ifa = ifa->ia_next) {
 	    afsi_SetServerIPRank(sa, ifa);
     }}
-#endif
-#endif				/* USEIFADDR */
-#ifndef USEIFADDR
+#   endif
+#  endif				/* USEIFADDR */
+#  ifndef USEIFADDR
     end:
-#endif
-#endif				/* AFS_SUN5_ENV */
-#endif				/* else AFS_USERSPACE_IP_ADDR */
+#  endif
+# endif				/* AFS_SUN5_ENV */
+#endif				/* AFS_USERSPACE_IP_ADDR */
     sa->sa_iprank += afs_randomMod15();
 
     return 0;
@@ -1521,7 +1527,9 @@ afs_GetCapabilities(struct server *ts)
     }
     /* InitCallBackStateN, triggered by our RPC, may need this */
     ReleaseWriteLock(&afs_xserver);
+    AFS_GUNLOCK();
     code = RXAFS_GetCapabilities(rxconn, &caps);
+    AFS_GLOCK();
     ObtainWriteLock(&afs_xserver, 723);
     /* we forced a conn above; important we mark it down if needed */
     if ((code < 0) && (code != RXGEN_OPCODE)) {
@@ -1799,8 +1807,6 @@ afs_GetServer(afs_uint32 *aserverp, afs_int32 nservers
 	if (afs_stats_cmperf.srvRecords > afs_stats_cmperf.srvRecordsHWM)
 	    afs_stats_cmperf.srvRecordsHWM = afs_stats_cmperf.srvRecords;
     }
-    /* We can't need this below, and won't reacquire */
-    ReleaseWriteLock(&afs_xvcb);
 
     ReleaseWriteLock(&afs_xsrvAddr);
 
