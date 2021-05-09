--- src/lwp/lock.h.orig	2021-01-14 21:08:41 UTC
+++ src/lwp/lock.h
@@ -167,15 +167,6 @@ void Lock_Destroy(struct Lock *lock);
 	    LOCK_UNLOCK(lock); \
 	ENDMAC
 
-#ifdef notdef
-/* this is what UnboostSharedLock looked like before the hi-C compiler */
-/* this must only be called with a WRITE or boosted SHARED lock! */
-#define UnboostSharedLock(lock)\
-	((lock)->excl_locked = SHARED_LOCK,\
-	((lock)->wait_states ?\
-		Afs_Lock_ReleaseR(lock) : 0))
-#endif /* notdef */
-
 #define ReleaseReadLock(lock)\
 	BEGINMAC\
 	    LOCK_LOCK(lock); \
@@ -185,15 +176,6 @@ void Lock_Destroy(struct Lock *lock);
 	ENDMAC
 
 
-#ifdef notdef
-/* This is what the previous definition should be, but the hi-C compiler generates
-  a warning for each invocation */
-#define ReleaseReadLock(lock)\
-	(!--(lock)->readers_reading && (lock)->wait_states ?\
-		Afs_Lock_ReleaseW(lock)    :\
-		0)
-#endif /* notdef */
-
 #define ReleaseWriteLock(lock)\
 	BEGINMAC\
 	    LOCK_LOCK(lock); \
@@ -202,15 +184,6 @@ void Lock_Destroy(struct Lock *lock);
 	    LOCK_UNLOCK(lock); \
 	ENDMAC
 
-#ifdef notdef
-/* This is what the previous definition should be, but the hi-C compiler generates
-   a warning for each invocation */
-#define ReleaseWriteLock(lock)\
-	((lock)->excl_locked &= ~WRITE_LOCK,\
-	((lock)->wait_states ?\
-		Afs_Lock_ReleaseR(lock) : 0))
-#endif /* notdef */
-
 /* can be used on shared or boosted (write) locks */
 #define ReleaseSharedLock(lock)\
 	BEGINMAC\
@@ -219,16 +192,6 @@ void Lock_Destroy(struct Lock *lock);
 	    if ((lock)->wait_states) Afs_Lock_ReleaseR(lock);\
 	    LOCK_UNLOCK(lock); \
 	ENDMAC
-
-#ifdef notdef
-/* This is what the previous definition should be, but the hi-C compiler generates
-   a warning for each invocation */
-/* can be used on shared or boosted (write) locks */
-#define ReleaseSharedLock(lock)\
-	((lock)->excl_locked &= ~(SHARED_LOCK | WRITE_LOCK),\
-	((lock)->wait_states ?\
-		Afs_Lock_ReleaseR(lock) : 0))
-#endif /* notdef */
 
 /* convert a write lock to a read lock */
 #define ConvertWriteToReadLock(lock)\
