--- src/config/param.amd64_fbsd_100.h.orig	2021-07-29 10:24:31 UTC
+++ src/config/param.amd64_fbsd_100.h
@@ -12,39 +12,9 @@
 
 #define AFS_64BITUSERPOINTER_ENV 1
 
-#define AFS_FBSD81_ENV 1
-#define AFS_FBSD82_ENV 1
-#define AFS_FBSD83_ENV 1
-#define AFS_FBSD84_ENV 1
-#define AFS_FBSD90_ENV 1
-#define AFS_FBSD91_ENV 1
-#define AFS_FBSD92_ENV 1
-#define AFS_FBSD93_ENV 1
-#define AFS_FBSD100_ENV 1
-
-#define AFS_X86_FBSD81_ENV 1
-#define AFS_X86_FBSD82_ENV 1
-#define AFS_X86_FBSD83_ENV 1
-#define AFS_X86_FBSD84_ENV 1
-#define AFS_X86_FBSD90_ENV 1
-#define AFS_X86_FBSD91_ENV 1
-#define AFS_X86_FBSD92_ENV 1
-#define AFS_X86_FBSD93_ENV 1
-#define AFS_X86_FBSD100_ENV 1
-
 #else /* !defined(UKERNEL) */
 
 /* This section for user space compiles only */
-
-#define AFS_USR_FBSD81_ENV 1
-#define AFS_USR_FBSD82_ENV 1
-#define AFS_USR_FBSD83_ENV 1
-#define AFS_USR_FBSD84_ENV 1
-#define AFS_USR_FBSD90_ENV 1
-#define AFS_USR_FBSD91_ENV 1
-#define AFS_USR_FBSD92_ENV 1
-#define AFS_USR_FBSD93_ENV 1
-#define AFS_USR_FBSD100_ENV 1
 
 #endif /* !defined(UKERNEL) */
 
