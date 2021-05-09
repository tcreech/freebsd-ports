--- src/rx/rx_clock.h.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_clock.h
@@ -190,7 +190,7 @@ extern void clock_UpdateTime(void);
                      + 2 * ((c2)->usec / 1000) * ((c2)->usec % 1000) / 1000   \
                      + ((((c2)->usec % 1000) > 707) ? 1 : 0);                 \
      }                                                                        \
-   if ((c1)->usec > 1000000) {                                                \
+   while ((c1)->usec > 1000000) {                                             \
         (c1)->usec -= 1000000;                                                \
         (c1)->sec++;                                                          \
    }                                                                          \
