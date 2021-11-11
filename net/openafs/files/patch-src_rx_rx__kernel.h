--- src/rx/rx_kernel.h.orig	2021-07-29 10:24:31 UTC
+++ src/rx/rx_kernel.h
@@ -16,7 +16,7 @@
 #define osi_Free  afs_osi_Free
 
 #ifndef RXK_TIMEDSLEEP_ENV
-#define rxi_ReScheduleEvents    0	/* Not needed by kernel */
+# define rxi_ReScheduleEvents    0	/* Not needed by kernel */
 #endif
 
 /* This is a no-op, because the kernel server procs are pre-allocated */
@@ -27,8 +27,8 @@ typedef struct socket *osi_socket;
 #define	OSI_NULLSOCKET	((osi_socket) 0)
 
 #if (!defined(AFS_GLOBAL_SUNLOCK) && !defined(RX_ENABLE_LOCKS))
-#include "afs/icl.h"
-#include "afs/afs_trace.h"
+# include "afs/icl.h"
+# include "afs/afs_trace.h"
 #endif
 #define osi_rxSleep(a)  afs_Trace2(afs_iclSetp, CM_TRACE_RXSLEEP, \
         ICL_TYPE_STRING, __FILE__, ICL_TYPE_INT32, __LINE__); afs_osi_Sleep(a)
@@ -37,26 +37,26 @@ typedef struct socket *osi_socket;
 
 extern int osi_utoa(char *buf, size_t len, unsigned long val);
 
-# if defined(AFS_LINUX26_ENV)
-#  define osi_Panic(msg...) \
+#if defined(AFS_LINUX26_ENV)
+# define osi_Panic(msg...) \
     do { printk(KERN_CRIT "openafs: " msg); BUG(); } while (0)
-#  define osi_Assert(expr) \
+# define osi_Assert(expr) \
     do { \
 	if (!(expr)) \
 	    osi_Panic("assertion failed: %s, file: %s, line: %d\n", \
 		      #expr, __FILE__, __LINE__); \
     } while (0)
-# elif defined(AFS_AIX_ENV)
+#elif defined(AFS_AIX_ENV)
 extern void osi_Panic(char *fmt, void *a1, void *a2, void *a3);
-#  define osi_Assert(exp) \
+# define osi_Assert(exp) \
     (void)((exp) || (osi_AssertFailK( #exp , __FILE__, __LINE__), 0))
-# else
+#else
 extern void osi_Panic(char *fmt, ...)
     AFS_ATTRIBUTE_FORMAT(__printf__, 1, 2)
     AFS_NORETURN;
-#  define osi_Assert(exp) \
+# define osi_Assert(exp) \
     (void)((exp) || (osi_AssertFailK( #exp , __FILE__, __LINE__), 0))
-# endif
+#endif
 
 #ifdef AFS_LINUX20_ENV
 # define	osi_Msg printk)(
@@ -68,45 +68,58 @@ extern void osi_Panic(char *fmt, ...)
 #define	osi_YieldIfPossible()
 #define	osi_WakeupAndYieldIfPossible(x)	    rx_Wakeup(x)
 
-#if !defined(AFS_DARWIN80_ENV) || defined(UKERNEL)
-# ifdef UKERNEL
-# define rx_ifnet_t struct usr_ifnet *
-# define rx_ifaddr_t struct usr_ifaddr *
+#ifdef AFS_FBSD120_ENV
+# ifdef AFS_FBSD_EXPLICIT_EPOCH_TRACKER
+#  define RX_NET_EPOCH_ENTER() struct epoch_tracker __et; NET_EPOCH_ENTER(__et)
+#  define RX_NET_EPOCH_EXIT() NET_EPOCH_EXIT(__et)
 # else
-# define rx_ifnet_t struct ifnet *
-# define rx_ifaddr_t struct ifaddr *
-#endif
-#define rx_ifnet_mtu(x) (x)->if_mtu
-#define rx_ifnet_flags(x) (x?(x)->if_flags:0)
-#if defined(FBSD_IFA_IFWITHNET_THREE_ARGS)
-#define rx_ifaddr_withnet(x) ifa_ifwithnet(x, 0, RT_ALL_FIBS)
-#elif defined(AFS_OBSD46_ENV) || defined(AFS_FBSD81_ENV)
-#define rx_ifaddr_withnet(x) ifa_ifwithnet(x, 0)
+#  define RX_NET_EPOCH_ENTER() NET_EPOCH_ENTER()
+#  define RX_NET_EPOCH_EXIT() NET_EPOCH_EXIT()
+# endif
 #else
-#define rx_ifaddr_withnet(x) ifa_ifwithnet(x)
+# define RX_NET_EPOCH_ENTER() do { } while(0)
+# define RX_NET_EPOCH_EXIT() do { } while(0)
 #endif
-#if defined(FBSD_IF_METRIC_IN_STRUCT_IFNET)
-#define rx_ifnet_metric(x) (x?(x)->if_metric:0)
-#else
-#define rx_ifnet_metric(x) (x?(x)->if_data.ifi_metric:0)
-#endif
-#define rx_ifaddr_ifnet(x) (x?(x)->ifa_ifp:0)
-#define rx_ifaddr_address_family(x) (x)->ifa_addr->sa_family
-#define rx_ifaddr_address(x, y, z) memcpy(y, (x)->ifa_addr, z)
-#define rx_ifaddr_netmask(x, y, z) memcpy(y, (x)->ifa_netmask, z)
-#define rx_ifaddr_dstaddress(x, y, z) memcpy(y, (x)->ifa_dstaddr, z)
-#else
-#define rx_ifnet_t ifnet_t
-#define rx_ifaddr_t ifaddr_t
-#define rx_ifaddr_withnet(x) ifaddr_withnet(x)
-#define rx_ifnet_mtu(x) ifnet_mtu(x)
-#define rx_ifnet_flags(x) ifnet_flags(x)
-#define rx_ifnet_metric(x) ifnet_metric(x)
-#define rx_ifaddr_ifnet(x) ifaddr_ifnet(x)
-#define rx_ifaddr_address_family(x) ifaddr_address_family(x)
-#define rx_ifaddr_address(x, y, z) ifaddr_address(x, y, z)
-#define rx_ifaddr_netmask(x, y, z) ifaddr_netmask(x, y, z)
-#define rx_ifaddr_dstaddress(x, y, z) ifaddr_dstaddress(x, y, z)
-#endif
+
+#if !defined(AFS_DARWIN80_ENV) || defined(UKERNEL)
+# ifdef UKERNEL
+#  define rx_ifnet_t struct usr_ifnet *
+#  define rx_ifaddr_t struct usr_ifaddr *
+# else
+#  define rx_ifnet_t struct ifnet *
+#  define rx_ifaddr_t struct ifaddr *
+# endif
+# define rx_ifnet_mtu(x) (x)->if_mtu
+# define rx_ifnet_flags(x) (x?(x)->if_flags:0)
+# if defined(FBSD_IFA_IFWITHNET_THREE_ARGS)
+#  define rx_ifaddr_withnet(x) ifa_ifwithnet(x, 0, RT_ALL_FIBS)
+# elif defined(AFS_OBSD46_ENV) || defined(AFS_FBSD_ENV)
+#  define rx_ifaddr_withnet(x) ifa_ifwithnet(x, 0)
+# else
+#  define rx_ifaddr_withnet(x) ifa_ifwithnet(x)
+# endif
+# if defined(FBSD_IF_METRIC_IN_STRUCT_IFNET)
+#  define rx_ifnet_metric(x) (x?(x)->if_metric:0)
+# else
+#  define rx_ifnet_metric(x) (x?(x)->if_data.ifi_metric:0)
+# endif
+# define rx_ifaddr_ifnet(x) (x?(x)->ifa_ifp:0)
+# define rx_ifaddr_address_family(x) (x)->ifa_addr->sa_family
+# define rx_ifaddr_address(x, y, z) memcpy(y, (x)->ifa_addr, z)
+# define rx_ifaddr_netmask(x, y, z) memcpy(y, (x)->ifa_netmask, z)
+# define rx_ifaddr_dstaddress(x, y, z) memcpy(y, (x)->ifa_dstaddr, z)
+#else /* !AFS_DARWIN80_ENV || UKERNEL */
+# define rx_ifnet_t ifnet_t
+# define rx_ifaddr_t ifaddr_t
+# define rx_ifaddr_withnet(x) ifaddr_withnet(x)
+# define rx_ifnet_mtu(x) ifnet_mtu(x)
+# define rx_ifnet_flags(x) ifnet_flags(x)
+# define rx_ifnet_metric(x) ifnet_metric(x)
+# define rx_ifaddr_ifnet(x) ifaddr_ifnet(x)
+# define rx_ifaddr_address_family(x) ifaddr_address_family(x)
+# define rx_ifaddr_address(x, y, z) ifaddr_address(x, y, z)
+# define rx_ifaddr_netmask(x, y, z) ifaddr_netmask(x, y, z)
+# define rx_ifaddr_dstaddress(x, y, z) ifaddr_dstaddress(x, y, z)
+#endif /* !AFS_DARWIN80_ENV || UKERNEL */
 
 #endif /* __RX_KERNEL_INCL_ */
