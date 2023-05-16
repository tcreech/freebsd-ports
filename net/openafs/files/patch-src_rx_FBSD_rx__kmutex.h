--- src/rx/FBSD/rx_kmutex.h.orig	2021-12-09 17:07:41 UTC
+++ src/rx/FBSD/rx_kmutex.h
@@ -56,11 +56,11 @@ typedef struct {
 
 #else /* NULL_LOCKS */
 
-typedef struct mtx afs_kmutex_t;
+typedef struct sx afs_kmutex_t;
 
 #ifdef WITNESS
 #define WITCLEAR_MTX(a)					\
-    do { memset((a), 0, sizeof(struct mtx)); } while(0)
+    do { memset((a), 0, sizeof(struct sx)); } while(0)
 #else
 #define WITCLEAR_MTX(a) {}
 #endif
@@ -68,29 +68,29 @@ typedef struct mtx afs_kmutex_t;
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
 
 #endif /* !NULL_LOCKS */
 
