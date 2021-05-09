--- src/rx/FBSD/rx_kmutex.h.orig	2021-01-14 21:08:41 UTC
+++ src/rx/FBSD/rx_kmutex.h
@@ -18,12 +18,8 @@
 
 #include <sys/systm.h>
 #include <sys/proc.h>
-#ifdef AFS_FBSD70_ENV
 #include <sys/lock.h>
 #include <sys/lockmgr.h>
-#else
-#include <sys/lock.h>
-#endif
 
 #define RX_ENABLE_LOCKS         1
 
@@ -58,13 +54,13 @@ typedef struct {
 
 #define MUTEX_ASSERT(a) osi_Assert(((afs_kmutex_t *)(a))->owner == curproc)
 
-#elif defined(AFS_FBSD70_ENV) /* dunno about 6.x */
+#else /* NULL_LOCKS */
 
-typedef struct mtx afs_kmutex_t;
+typedef struct sx afs_kmutex_t;
 
-#if defined(AFS_FBSD80_ENV) && defined(WITNESS)
+#ifdef WITNESS
 #define WITCLEAR_MTX(a)					\
-    do { memset((a), 0, sizeof(struct mtx)); } while(0)
+    do { memset((a), 0, sizeof(struct sx)); } while(0)
 #else
 #define WITCLEAR_MTX(a) {}
 #endif
@@ -72,67 +68,32 @@ typedef struct mtx afs_kmutex_t;
 #define MUTEX_INIT(a,b,c,d)					     \
   do {								     \
       WITCLEAR_MTX(a);						     \
-      mtx_init((a), (b), 0 /* type defaults to name */, MTX_DEF | MTX_DUPOK);    \
+      sx_init((a), (b)); \
   } while(0)
 
 #define MUTEX_DESTROY(a)			\
     do {					\
-	mtx_destroy((a));			\
+	sx_destroy((a));			\
     } while(0)
 
 #define MUTEX_ENTER(a) \
     do {	       \
-	mtx_lock((a)); \
+	sx_xlock((a)); \
     } while(0)
 
 #define MUTEX_TRYENTER(a)			\
-    ( mtx_trylock((a)) )
+    ( sx_try_xlock((a)) )
 
 #define MUTEX_EXIT(a)	 \
     do {		 \
-	mtx_unlock((a)); \
+	sx_xunlock((a)); \
     } while(0)
 
 #define MUTEX_ASSERT(a)				\
-    osi_Assert(mtx_owned((a)))
+    osi_Assert(sx_xlocked((a)))
 
-#else
+#endif /* !NULL_LOCKS */
 
-typedef struct {
-    struct lock lock;
-    struct thread *owner;
-} afs_kmutex_t;
-
-
-#define MUTEX_INIT(a,b,c,d) \
-    do { \
-	lockinit(&(a)->lock,PSOCK, "afs rx mutex", 0, 0); \
-	(a)->owner = 0; \
-    } while(0)
-#define MUTEX_DESTROY(a) \
-    do { \
-	(a)->owner = (struct proc *)-1; \
-    } while(0)
-#define MUTEX_ENTER(a) \
-    do { \
-	lockmgr(&(a)->lock, LK_EXCLUSIVE, 0, curthread); \
-	osi_Assert((a)->owner == 0); \
-	(a)->owner = curthread; \
-    } while(0)
-#define MUTEX_TRYENTER(a) \
-    ( lockmgr(&(a)->lock, LK_EXCLUSIVE|LK_NOWAIT, 0, curthread) ? 0 : ((a)->owner = curthread, 1) )
-#define xMUTEX_TRYENTER(a) \
-    ( osi_Assert((a)->owner == 0), (a)->owner = curthread, 1)
-#define MUTEX_EXIT(a) \
-    do { \
-	osi_Assert((a)->owner == curthread); \
-	(a)->owner = 0; \
-	lockmgr(&(a)->lock, LK_RELEASE, 0, curthread); \
-    } while(0)
-
-#define MUTEX_ASSERT(a) osi_Assert(((afs_kmutex_t *)(a))->owner == curthread)
-#endif
-
 /*
  * Condition variables
  *
@@ -143,8 +104,6 @@ typedef struct {
 #define CV_INIT(cv,a,b,c)
 #define CV_DESTROY(cv)
 
-#if defined(AFS_FBSD70_ENV)
-
 #define CV_WAIT(cv, lck)    { \
     int isGlockOwner = ISAFS_GLOCK();					\
     if (isGlockOwner) AFS_GUNLOCK();					\
@@ -158,26 +117,6 @@ typedef struct {
 	                        msleep(cv, lck, PSOCK, "afs_rx_cv_timedwait", t); \
 	                        if (isGlockOwner) AFS_GLOCK();  \
 				}
-#else /* !AFS_FBSD70_ENV */
-#define CV_WAIT(cv, lck)    { \
-	                        int isGlockOwner = ISAFS_GLOCK(); \
-	                        if (isGlockOwner) AFS_GUNLOCK();  \
-	                        MUTEX_EXIT(lck);        \
-	                        tsleep(cv, PSOCK, "afs_rx_cv_wait", 0);  \
-	                        if (isGlockOwner) AFS_GLOCK();  \
-	                        MUTEX_ENTER(lck); \
-	                    }
-
-#define CV_TIMEDWAIT(cv,lck,t)  { \
-	                        int isGlockOwner = ISAFS_GLOCK(); \
-	                        if (isGlockOwner) AFS_GUNLOCK();  \
-	                        MUTEX_EXIT(lck);        \
-	                        tsleep(cv, PSOCK, "afs_rx_cv_timedwait", t); \
-	                        if (isGlockOwner) AFS_GLOCK();  \
-	                        MUTEX_ENTER(lck);       \
-				}
-#endif /* AFS_FBSD80_ENV */
-
 #define CV_SIGNAL(cv)           wakeup_one(cv)
 #define CV_BROADCAST(cv)        wakeup(cv)
 
