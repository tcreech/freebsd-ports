--- src/config/param.generic_fbsd.h.orig	2016-12-08 04:01:51 UTC
+++ src/config/param.generic_fbsd.h
@@ -127,6 +127,16 @@ enum vcexcl { NONEXCL, EXCL };
 #define FBSD_SYSCALL_REGISTER_FOUR_ARGS
 #endif
 
+/* r285819 eliminated b_saveaddr from struct buf */
+#if __FreeBSD_version >= 1100078
+#define FBSD_STRUCT_BUF_NO_SAVEADDR
+#endif
+
+/* r292373 changed the KPI for VOP_GETPAGES */
+#if __FreeBSD_version >= 1100092
+#define FBSD_VOP_GETPAGES_BUSIED
+#endif
+
 #else /* !defined(UKERNEL) */
 
 /* This section for user space compiles only */
