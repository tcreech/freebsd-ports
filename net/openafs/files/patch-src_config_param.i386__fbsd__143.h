--- src/config/param.i386_fbsd_143.h.orig	2025-06-22 01:33:01 UTC
+++ src/config/param.i386_fbsd_143.h
@@ -0,0 +1,72 @@
+#ifndef	AFS_PARAM_H
+#define	AFS_PARAM_H
+
+/* Machine / Operating system information */
+#define SYS_NAME	"i386_fbsd_143"
+#define SYS_NAME_ID	SYS_NAME_ID_i386_fbsd_143
+
+#define AFS_FAKEOPEN_ENV 1	/* call afs_FakeOpen as if !AFS_VM_RDWR */
+
+#ifndef UKERNEL
+/* This section for kernel libafs compiles only */
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
+#define AFS_FBSD141_ENV 1
+#define AFS_FBSD142_ENV 1
+#define AFS_FBSD143_ENV 1
+
+#define AFS_X86_FBSD101_ENV 1
+#define AFS_X86_FBSD102_ENV 1
+#define AFS_X86_FBSD103_ENV 1
+#define AFS_X86_FBSD104_ENV 1
+#define AFS_X86_FBSD110_ENV 1
+#define AFS_X86_FBSD111_ENV 1
+#define AFS_X86_FBSD120_ENV 1
+#define AFS_X86_FBSD121_ENV 1
+#define AFS_X86_FBSD122_ENV 1
+#define AFS_X86_FBSD130_ENV 1
+#define AFS_X86_FBSD131_ENV 1
+#define AFS_X86_FBSD132_ENV 1
+#define AFS_X86_FBSD140_ENV 1
+#define AFS_X86_FBSD141_ENV 1
+#define AFS_X86_FBSD142_ENV 1
+#define AFS_X86_FBSD143_ENV 1
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
+#define AFS_USR_FBSD141_ENV 1
+#define AFS_USR_FBSD143_ENV 1
+
+#endif /* !defined(UKERNEL) */
+
+#endif /* AFS_PARAM_H */
