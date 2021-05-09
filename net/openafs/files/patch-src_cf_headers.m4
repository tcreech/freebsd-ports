--- src/cf/headers.m4.orig	2021-01-14 21:08:41 UTC
+++ src/cf/headers.m4
@@ -88,7 +88,8 @@ AC_CHECK_HEADERS([security/pam_modules.h],[],[],[AC_IN
 # include <security/pam_appl.h>
 #endif])
 
-AC_CHECK_HEADERS(linux/errqueue.h,,,[#include <linux/types.h>])
+AC_CHECK_HEADERS(linux/errqueue.h,,,[#include <linux/types.h>
+#include <linux/time.h>])
 
 AC_CHECK_TYPES([fsblkcnt_t],,,[
 #include <sys/types.h>
