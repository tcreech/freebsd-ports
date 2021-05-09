--- src/viced/fs_stats.h.orig	2021-01-14 21:08:41 UTC
+++ src/viced/fs_stats.h
@@ -286,7 +286,7 @@ struct fs_stats_xferData {
       {                                                      \
        t1.tv_usec += (int) ((0.000001 * t2.tv_usec) * t2.tv_usec);   \
       }                                                      \
-    if (t1.tv_usec > 1000000) {                              \
+    while (t1.tv_usec > 1000000) {                           \
         t1.tv_usec -= 1000000;                               \
         t1.tv_sec++;                                         \
     }                                                        \
