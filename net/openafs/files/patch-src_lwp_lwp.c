--- src/lwp/lwp.c.orig	2021-01-14 21:08:41 UTC
+++ src/lwp/lwp.c
@@ -34,7 +34,7 @@ int setlim(int limcon, uchar_t hard, int limit);
 #endif
 
 #ifndef AFS_ARM_LINUX20_ENV
-#if defined(AFS_OSF_ENV) || defined(AFS_S390_LINUX20_ENV)
+#if defined(AFS_S390_LINUX20_ENV)
 int PRE_Block;	/* Remnants of preemption support. */
 #else
 char PRE_Block;	/* Remnants of preemption support. */
