--- src/config/param.generic_fbsd.h.orig	2018-02-19 14:40:36 UTC
+++ src/config/param.generic_fbsd.h
@@ -136,6 +136,11 @@ enum vcexcl { NONEXCL, EXCL };
 #define FBSD_VOP_GETPAGES_BUSIED
 #endif
 
+/* r316528 added a V_VMIO flag for vinvalbuf */
+#if __FreeBSD_version >= 1101001
+#define FBSD_VINVALBUF_HAS_VMIO
+#endif
+
 #else /* !defined(UKERNEL) */
 
 /* This section for user space compiles only */
