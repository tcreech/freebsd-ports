--- src/rx/FBSD/rx_kmutex.h.orig	2016-11-30 20:06:42 UTC
+++ src/rx/FBSD/rx_kmutex.h
@@ -19,7 +19,9 @@
 #include <sys/systm.h>
 #include <sys/proc.h>
 #ifdef AFS_FBSD70_ENV
+#include <sys/param.h>
 #include <sys/lock.h>
+#include <sys/mutex.h>
 #include <sys/lockmgr.h>
 #else
 #include <sys/lock.h>
@@ -28,7 +30,8 @@
 #define RX_ENABLE_LOCKS         1
 #define AFS_GLOBAL_RXLOCK_KERNEL
 
-typedef int afs_kcondvar_t;
+typedef struct mtx afs_kmutex_t;
+typedef struct cv afs_kcondvar_t;
 
 #define HEAVY_LOCKS
 #if defined(NULL_LOCKS)
@@ -145,28 +148,17 @@ extern void osirx_AssertMine(afs_kmutex_
 /*
  * Condition variables
  *
- * In Digital Unix (OSF/1), we use something akin to the ancient sleep/wakeup
- * mechanism.  The condition variable itself plays no role; we just use its
- * address as a convenient unique number.
  */
-#define CV_INIT(cv,a,b,c)
-#define CV_DESTROY(cv)
+#define CV_INIT(cv,a,b,c)   cv_init(cv, a)
+#define CV_DESTROY(cv)      cv_destroy(cv)
 
 #if defined(AFS_FBSD70_ENV)
 
-#define CV_WAIT(cv, lck)    { \
-    int isGlockOwner = ISAFS_GLOCK();					\
-    if (isGlockOwner) AFS_GUNLOCK();					\
-    msleep(cv, lck, PSOCK, "afs_rx_cv_wait", 0);			\
-    if (isGlockOwner) AFS_GLOCK();					\
-  }
+#define CV_WAIT(cv, lck)    afs_cv_wait(cv, lck, 0)
+#define CV_WAIT_SIG(cv, lck)    afs_cv_wait(cv, lck, 1)
+
+#define CV_TIMEDWAIT(cv,lck,t)  afs_cv_timedwait(cv, m, t, 0)
 
-#define CV_TIMEDWAIT(cv,lck,t)  { \
-	                        int isGlockOwner = ISAFS_GLOCK(); \
-	                        if (isGlockOwner) AFS_GUNLOCK();  \
-	                        msleep(cv, lck, PSOCK, "afs_rx_cv_timedwait", t); \
-	                        if (isGlockOwner) AFS_GLOCK();  \
-				}
 #else /* !AFS_FBSD70_ENV */
 #define CV_WAIT(cv, lck)    { \
 	                        int isGlockOwner = ISAFS_GLOCK(); \
@@ -187,8 +179,8 @@ extern void osirx_AssertMine(afs_kmutex_
 				}
 #endif /* AFS_FBSD80_ENV */
 
-#define CV_SIGNAL(cv)           wakeup_one(cv)
-#define CV_BROADCAST(cv)        wakeup(cv)
+#define CV_SIGNAL(cv)           cv_signal(cv)
+#define CV_BROADCAST(cv)        cv_broadcast(cv)
 
 /* #define osi_rxWakeup(cv)        wakeup(cv) */
 
