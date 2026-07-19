--- src/rx/FBSD/rx_knet.c.orig	2026-02-26 17:15:49 UTC
+++ src/rx/FBSD/rx_knet.c
@@ -14,6 +14,8 @@
 
 #include <sys/malloc.h>
 #include "rx/rx_kcommon.h"
+#include "rx/rx_atomic.h"
+#include "rx/rx_internal.h"
 
 #ifdef RXK_LISTENER_ENV
 int
