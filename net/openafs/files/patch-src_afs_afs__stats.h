--- src/afs/afs_stats.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_stats.h
@@ -1067,7 +1067,7 @@ struct afs_stats_xferData {
                      + 2 * (t2.tv_usec / 1000) * (t2.tv_usec % 1000) / 1000   \
                      + (((t2.tv_usec % 1000) > 707) ? 1 : 0);                 \
      }                                                                        \
-   if (t1.tv_usec > 1000000) {                                                \
+   while (t1.tv_usec > 1000000) {                                             \
         t1.tv_usec -= 1000000;                                                \
         t1.tv_sec++;                                                          \
    }                                                                          \
