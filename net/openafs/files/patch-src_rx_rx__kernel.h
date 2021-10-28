--- src/rx/rx_kernel.h.orig	2021-07-29 10:24:31 UTC
+++ src/rx/rx_kernel.h
@@ -68,6 +68,19 @@ extern void osi_Panic(char *fmt, ...)
 #define	osi_YieldIfPossible()
 #define	osi_WakeupAndYieldIfPossible(x)	    rx_Wakeup(x)
 
+#ifdef AFS_FBSD120_ENV
+# ifdef AFS_FBSD_EXPLICIT_EPOCH_TRACKER
+#  define RX_NET_EPOCH_ENTER() struct epoch_tracker __et; NET_EPOCH_ENTER(__et)
+#  define RX_NET_EPOCH_EXIT() NET_EPOCH_EXIT(__et)
+# else
+#  define RX_NET_EPOCH_ENTER() NET_EPOCH_ENTER()
+#  define RX_NET_EPOCH_EXIT() NET_EPOCH_EXIT()
+# endif
+#else
+# define RX_NET_EPOCH_ENTER() do { } while(0)
+# define RX_NET_EPOCH_EXIT() do { } while(0)
+#endif
+
 #if !defined(AFS_DARWIN80_ENV) || defined(UKERNEL)
 # ifdef UKERNEL
 # define rx_ifnet_t struct usr_ifnet *
@@ -80,7 +93,7 @@ extern void osi_Panic(char *fmt, ...)
 #define rx_ifnet_flags(x) (x?(x)->if_flags:0)
 #if defined(FBSD_IFA_IFWITHNET_THREE_ARGS)
 #define rx_ifaddr_withnet(x) ifa_ifwithnet(x, 0, RT_ALL_FIBS)
-#elif defined(AFS_OBSD46_ENV) || defined(AFS_FBSD81_ENV)
+#elif defined(AFS_OBSD46_ENV) || defined(AFS_FBSD_ENV)
 #define rx_ifaddr_withnet(x) ifa_ifwithnet(x, 0)
 #else
 #define rx_ifaddr_withnet(x) ifa_ifwithnet(x)
