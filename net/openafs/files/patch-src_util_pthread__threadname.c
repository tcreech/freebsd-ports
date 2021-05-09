--- src/util/pthread_threadname.c.orig	2021-01-14 21:08:41 UTC
+++ src/util/pthread_threadname.c
@@ -26,7 +26,7 @@
 #include <afsconfig.h>
 #include <afs/param.h>
 
-#include "afsutil.h"
+#include <afs/opr.h>
 
 #if defined(AFS_PTHREAD_ENV) && !defined(AFS_NT40_ENV)
 # include <pthread.h>
@@ -35,24 +35,24 @@
 # endif
 
 void
-afs_pthread_setname_self(const char *threadname)
+opr_threadname_set(const char *threadname)
 {
 # if defined(HAVE_PTHREAD_SET_NAME_NP)
-	/* FreeBSD style */
-	pthread_set_name_np(pthread_self(), threadname);
+    /* FreeBSD style */
+    pthread_set_name_np(pthread_self(), threadname);
 # elif defined(HAVE_PTHREAD_SETNAME_NP)
 #  if PTHREAD_SETNAME_NP_ARGS == 3
-	/* DECthreads style */
-	pthread_setname_np(pthread_self(), threadname, (void *)0);
+    /* DECthreads style */
+    pthread_setname_np(pthread_self(), threadname, (void *)0);
 #  elif PTHREAD_SETNAME_NP_ARGS == 2
-	/* GNU libc on Linux style */
-	pthread_setname_np(pthread_self(), threadname);
+    /* GNU libc on Linux style */
+    pthread_setname_np(pthread_self(), threadname);
 #  elif PTHREAD_SETNAME_NP_ARGS == 1
-	/* Mac OS style */
-	pthread_setname_np(threadname);
+    /* Mac OS style */
+    pthread_setname_np(threadname);
 #  else
-#    error "Could not identify your pthread_setname_np() implementation"
-#  endif
-# endif
+#   error "Could not identify your pthread_setname_np() implementation"
+#  endif /* PTHREAD_SETNAME_NP_ARGS */
+# endif /* HAVE_PTHREAD_SETNAME_NP */
 }
-#endif
+#endif /* AFS_PTHREAD_ENV && !AFS_NT40_ENV */
