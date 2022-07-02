--- src/afs/FBSD/osi_machdep.h.orig	2021-12-09 17:07:41 UTC
+++ src/afs/FBSD/osi_machdep.h
@@ -22,6 +22,7 @@
 #include <sys/lock.h>
 #include <sys/time.h>
 #include <sys/mutex.h>
+#include <sys/sx.h>
 #include <sys/vnode.h>
 #include <sys/priv.h>
 
