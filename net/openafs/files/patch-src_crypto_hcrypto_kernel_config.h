--- src/crypto/hcrypto/kernel/config.h.orig	2021-01-14 21:08:41 UTC
+++ src/crypto/hcrypto/kernel/config.h
@@ -60,6 +60,9 @@
 #define calloc _afscrypto_calloc
 void * _afscrypto_calloc(int, size_t);
 
+#ifdef malloc
+# undef malloc
+#endif
 #define malloc _afscrypto_malloc
 void * _afscrypto_malloc(size_t);
 
