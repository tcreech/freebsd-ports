--- src/rx/FBSD/rx_kmutex.h.orig	2017-12-05 14:43:14 UTC
+++ src/rx/FBSD/rx_kmutex.h
@@ -38,25 +38,28 @@ typedef struct {
 #define MUTEX_INIT(a,b,c,d) \
     do { \
 	(a)->owner = 0; \
-    } while(0);
+    } while(0)
 #define MUTEX_DESTROY(a) \
     do { \
 	(a)->owner = (struct proc *)-1; \
-    } while(0);
+    } while(0)
 #define MUTEX_ENTER(a) \
     do { \
 	osi_Assert((a)->owner == 0); \
 	(a)->owner = curproc; \
-    } while(0);
+    } while(0)
 #define MUTEX_TRYENTER(a) \
     ( osi_Assert((a)->owner == 0), (a)->owner = curproc, 1)
 #define MUTEX_EXIT(a) \
     do { \
 	osi_Assert((a)->owner == curproc); \
 	(a)->owner = 0; \
-    } while(0);
+    } while(0)
 
-#define MUTEX_ASSERT(a) osi_Assert(((afs_kmutex_t *)(a))->owner == curproc)
+#define MUTEX_ASSERT(a) \
+    do { \
+	osi_Assert(((afs_kmutex_t *)(a))->owner == curproc); \
+    } while(0)
 
 #elif defined(AFS_FBSD70_ENV) /* dunno about 6.x */
 
@@ -64,7 +67,7 @@ typedef struct mtx afs_kmutex_t;
 
 #if defined(AFS_FBSD80_ENV) && defined(WITNESS)
 #define WITCLEAR_MTX(a)					\
-    do { memset((a), 0, sizeof(struct mtx)); } while(0);
+    do { memset((a), 0, sizeof(struct mtx)); } while(0)
 #else
 #define WITCLEAR_MTX(a) {}
 #endif
@@ -73,17 +76,17 @@ typedef struct mtx afs_kmutex_t;
   do {								     \
       WITCLEAR_MTX(a);						     \
       mtx_init((a), (b), 0 /* type defaults to name */, MTX_DEF | MTX_DUPOK);    \
-  } while(0);
+  } while(0)
 
 #define MUTEX_DESTROY(a)			\
     do {					\
 	mtx_destroy((a));			\
-    } while(0);
+    } while(0)
 
 #define MUTEX_ENTER(a) \
     do {	       \
 	mtx_lock((a)); \
-    } while(0);
+    } while(0)
 
 #define MUTEX_TRYENTER(a)			\
     ( mtx_trylock((a)) )
@@ -91,10 +94,12 @@ typedef struct mtx afs_kmutex_t;
 #define MUTEX_EXIT(a)	 \
     do {		 \
 	mtx_unlock((a)); \
-    } while(0);
+    } while(0)
 
-#define MUTEX_ASSERT(a)				\
-    osi_Assert(mtx_owned((a)))
+#define MUTEX_ASSERT(a)                         \
+    do {                                        \
+	osi_Assert(mtx_owned((a)));             \
+    } while(0)
 
 #else
 
@@ -108,17 +113,17 @@ typedef struct {
     do { \
 	lockinit(&(a)->lock,PSOCK, "afs rx mutex", 0, 0); \
 	(a)->owner = 0; \
-    } while(0);
+    } while(0)
 #define MUTEX_DESTROY(a) \
     do { \
 	(a)->owner = (struct proc *)-1; \
-    } while(0);
+    } while(0)
 #define MUTEX_ENTER(a) \
     do { \
 	lockmgr(&(a)->lock, LK_EXCLUSIVE, 0, curthread); \
 	osi_Assert((a)->owner == 0); \
 	(a)->owner = curthread; \
-    } while(0);
+    } while(0)
 #define MUTEX_TRYENTER(a) \
     ( lockmgr(&(a)->lock, LK_EXCLUSIVE|LK_NOWAIT, 0, curthread) ? 0 : ((a)->owner = curthread, 1) )
 #define xMUTEX_TRYENTER(a) \
@@ -128,9 +133,13 @@ typedef struct {
 	osi_Assert((a)->owner == curthread); \
 	(a)->owner = 0; \
 	lockmgr(&(a)->lock, LK_RELEASE, 0, curthread); \
-    } while(0);
+    } while(0)
+
+#define MUTEX_ASSERT(a) \
+    do { \
+	osi_Assert(((afs_kmutex_t *)(a))->owner == curthread); \
+    } while(0)
 
-#define MUTEX_ASSERT(a) osi_Assert(((afs_kmutex_t *)(a))->owner == curthread)
 #endif
 
 /*
