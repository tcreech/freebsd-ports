--- src/config/param.arm64_fbsd_140.h.orig	2023-11-20 15:23:35 UTC
+++ src/config/param.arm64_fbsd_140.h
@@ -0,0 +1,54 @@
+#ifndef	AFS_PARAM_H
+#define	AFS_PARAM_H
+
+/* Machine / Operating system information */
+#define SYS_NAME	"arm64_fbsd_140"
+#define SYS_NAME_ID	SYS_NAME_ID_arm64_fbsd_140
+
+#define AFS_64BITPOINTER_ENV 1
+
+#ifndef UKERNEL
+/* This section for kernel libafs compiles only */
+
+#define AFS_64BITUSERPOINTER_ENV 1
+
+/* Request definitions of deprecated class A/B/C networks */
+#define IN_HISTORICAL_NETS
+
+#define AFS_FBSD101_ENV 1
+#define AFS_FBSD102_ENV 1
+#define AFS_FBSD103_ENV 1
+#define AFS_FBSD104_ENV 1
+#define AFS_FBSD110_ENV 1
+#define AFS_FBSD111_ENV 1
+#define AFS_FBSD120_ENV 1
+#define AFS_FBSD121_ENV 1
+#define AFS_FBSD122_ENV 1
+#define AFS_FBSD130_ENV 1
+#define AFS_FBSD131_ENV 1
+#define AFS_FBSD132_ENV 1
+#define AFS_FBSD140_ENV 1
+
+#else /* !defined(UKERNEL) */
+
+/* This section for user space compiles only */
+
+#define AFS_USR_FBSD101_ENV 1
+#define AFS_USR_FBSD102_ENV 1
+#define AFS_USR_FBSD103_ENV 1
+#define AFS_USR_FBSD104_ENV 1
+#define AFS_USR_FBSD110_ENV 1
+#define AFS_USR_FBSD111_ENV 1
+#define AFS_USR_FBSD120_ENV 1
+#define AFS_USR_FBSD121_ENV 1
+#define AFS_USR_FBSD122_ENV 1
+#define AFS_USR_FBSD130_ENV 1
+#define AFS_USR_FBSD131_ENV 1
+#define AFS_USR_FBSD132_ENV 1
+#define AFS_USR_FBSD140_ENV 1
+
+#endif /* !defined(UKERNEL) */
+
+#define USE_UCONTEXT
+
+#endif /* AFS_PARAM_H */
