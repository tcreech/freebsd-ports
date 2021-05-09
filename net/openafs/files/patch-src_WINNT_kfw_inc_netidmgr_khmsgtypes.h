--- src/WINNT/kfw/inc/netidmgr/khmsgtypes.h.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/kfw/inc/netidmgr/khmsgtypes.h
@@ -462,14 +462,6 @@
  */
 #define KMSG_CRED_DESTROY_CREDS     32
 
-#if 0
-/*! \brief Parse an identity
-
-    \note May be sent to individual credential subscriptions.
- */
-#define KMSG_CRED_IDENT_PARSE       65
-#endif
-
 /*! \brief A property page is being launced
 
     Message parameters:
