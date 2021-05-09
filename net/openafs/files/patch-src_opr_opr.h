--- src/opr/opr.h.orig	2021-01-14 21:08:41 UTC
+++ src/opr/opr.h
@@ -75,4 +75,16 @@ extern void opr_stolower(char *s) AFS_NONNULL(());
 extern void opr_stoupper(char *s) AFS_NONNULL(());
 extern char *opr_strcompose(char *buf, size_t len, ...) AFS_NONNULL((1));
 
+/* threadname.c */
+
+#if defined(AFS_PTHREAD_ENV) && !defined(AFS_NT40_ENV)
+extern void opr_threadname_set(const char *threadname);
+#else
+static_inline void
+opr_threadname_set(const char *threadname)
+{
+    /* noop */
+}
+#endif
+
 #endif
