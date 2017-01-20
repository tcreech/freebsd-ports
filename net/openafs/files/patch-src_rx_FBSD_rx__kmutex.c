--- src/rx/FBSD/rx_kmutex.c.orig	2016-11-30 20:06:42 UTC
+++ src/rx/FBSD/rx_kmutex.c
@@ -16,7 +16,66 @@
 #include <afsconfig.h>
 #include "afs/param.h"
 
+#include "rx/rx_kcommon.h"
+#include "rx_kmutex.h"
+#include "rx/rx_kernel.h"
 
-/*
- * Currently everything is implemented in rx_kmutex.h
- */
+#include <sys/param.h>
+#include <sys/lock.h>
+#include <sys/mutex.h>
+
+int
+afs_cv_wait(cv, m, sigok)
+    afs_kcondvar_t *cv;
+    afs_kmutex_t *m;
+    int sigok;
+{
+    int haveGlock = ISAFS_GLOCK();
+    int retval = 0;
+
+    if (haveGlock)
+        AFS_GUNLOCK();
+
+    if (sigok) {
+        if (cv_wait_sig(cv, m) == 0)
+            retval = EINTR;
+    } else {
+        cv_wait(cv, m);
+    }
+
+    if (haveGlock) {
+        MUTEX_EXIT(m);
+        AFS_GLOCK();
+        MUTEX_ENTER(m);
+    }
+
+    return retval;
+}
+
+afs_cv_timedwait(cv, m, t, sigok)
+    afs_kcondvar_t *cv;
+    afs_kmutex_t *m;
+    // The time "t" may not have the same meaning as FreeBSD's "timo"!
+    int t;
+    int sigok;
+{
+    int haveGlock = ISAFS_GLOCK();
+    int retval = 0;
+
+    if (haveGlock)
+        AFS_GUNLOCK();
+
+    if (sigok) {
+        if (cv_timedwait_sig(cv, m, t) == 0)
+            retval = EINTR;
+    } else {
+        cv_timedwait(cv, m, t);
+    }
+
+    if (haveGlock) {
+        MUTEX_EXIT(m);
+        AFS_GLOCK();
+        MUTEX_ENTER(m);
+    }
+    return retval;
+}
