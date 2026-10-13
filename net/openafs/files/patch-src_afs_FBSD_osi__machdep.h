--- src/afs/FBSD/osi_machdep.h.orig	2025-01-23 17:12:55 UTC
+++ src/afs/FBSD/osi_machdep.h
@@ -22,8 +22,10 @@
 #include <sys/lock.h>
 #include <sys/time.h>
 #include <sys/mutex.h>
+#include <sys/sx.h>
 #include <sys/vnode.h>
 #include <sys/priv.h>
+#include <vm/uma.h>
 
 /*
  * Time related macros
