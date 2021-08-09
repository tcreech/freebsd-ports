--- src/afsweb/apache_includes/1.3.6/ap_config.h.orig	2021-07-29 10:24:31 UTC
+++ src/afsweb/apache_includes/1.3.6/ap_config.h
@@ -682,8 +682,7 @@ stat() properly */
 #ifndef DEFAULT_GROUP
 #define DEFAULT_GROUP "nogroup"
 #endif
-#if defined(__bsdi__) || \
-(defined(__FreeBSD_version) && (__FreeBSD_version < 220000))
+#if defined(__bsdi__)
     typedef quad_t rlim_t;
 #endif
 #define USE_FLOCK_SERIALIZED_ACCEPT
