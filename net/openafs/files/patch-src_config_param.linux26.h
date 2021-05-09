--- src/config/param.linux26.h.orig	2021-01-14 21:08:41 UTC
+++ src/config/param.linux26.h
@@ -105,7 +105,7 @@
 #include <features.h>
 #endif
 
-#if defined(HAVE_LINUX_ERRQUEUE_H) && defined(HAVE_SETSOCKOPT_IP_RECVERR)
+#if defined(HAVE_LINUX_ERRQUEUE_H) && defined(HAVE_SETSOCKOPT_IP_RECVERR) && !defined(UKERNEL)
 # define AFS_RXERRQ_ENV
 #endif
 #ifdef AFS_RXERRQ_ENV
