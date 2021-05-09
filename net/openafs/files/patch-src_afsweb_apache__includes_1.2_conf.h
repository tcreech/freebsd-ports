--- src/afsweb/apache_includes/1.2/conf.h.orig	2021-01-14 21:08:41 UTC
+++ src/afsweb/apache_includes/1.2/conf.h
@@ -404,8 +404,7 @@ extern char *crypt();
 #define HAVE_MMAP
 #define DEFAULT_USER "nobody"
 #define DEFAULT_GROUP "nogroup"
-#if defined(__bsdi__) || \
-(defined(__FreeBSD_version) && (__FreeBSD_version < 220000))
+#if defined(__bsdi__)
 typedef quad_t rlim_t;
 #endif
 #define USE_FLOCK_SERIALIZED_ACCEPT
