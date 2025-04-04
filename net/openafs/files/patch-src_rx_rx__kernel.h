--- src/rx/rx_kernel.h.orig	2024-10-03 22:32:45 UTC
+++ src/rx/rx_kernel.h
@@ -66,8 +66,13 @@ extern void osi_Panic(char *fmt, ...)
 #define	osi_WakeupAndYieldIfPossible(x)	    rx_Wakeup(x)
 
 #ifdef AFS_FBSD120_ENV
-# define RX_NET_EPOCH_ENTER() NET_EPOCH_ENTER()
-# define RX_NET_EPOCH_EXIT() NET_EPOCH_EXIT()
+# ifdef AFS_FBSD_EXPLICIT_EPOCH_TRACKER
+#  define RX_NET_EPOCH_ENTER() struct epoch_tracker __et; NET_EPOCH_ENTER(__et)
+#  define RX_NET_EPOCH_EXIT() NET_EPOCH_EXIT(__et)
+# else
+#  define RX_NET_EPOCH_ENTER() NET_EPOCH_ENTER()
+#  define RX_NET_EPOCH_EXIT() NET_EPOCH_EXIT()
+# endif
 #else
 # define RX_NET_EPOCH_ENTER() do { } while(0)
 # define RX_NET_EPOCH_EXIT() do { } while(0)
