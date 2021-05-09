--- src/rx/HPUX/rx_kmutex.h.orig	2021-01-14 21:08:41 UTC
+++ src/rx/HPUX/rx_kmutex.h
@@ -73,48 +73,6 @@ extern lock_t *rx_sleepLock;
 	} while(0)
 
 
-#if 0
-#define CV_WAIT(cv, lck) \
-    do { \
-        int code; \
-        ksleep_prepare(); \
-        MP_SPINLOCK(rx_sleepLock); \
-        if (!b_owns_sema(lck)) \
-            osi_Panic("mutex not held \n"); \
-        b_vsema(lck); \
-        code = ksleep_one(PCATCH | KERNEL_ADDRESS | KERN_SPINLOCK_OBJECT, \
-            (cv), rx_sleepLock, 0); \
-        if (code) { \
-            if (code == EINTR) { /* lock still held */ \
-                MP_SPINUNLOCK(rx_sleepLock); \
-            } else if (code != -EINTR) { \
-                osi_Panic("ksleep_one failed: code = %d \n", code); \
-            } \
-        } \
-        b_psema(lck); /* grab the mutex again */ \
-    } while(0)
-
-/* Wakes up a thread waiting on this condition */
-#define CV_SIGNAL(cv) \
-    do { \
-        int wo, code; \
-        MP_SPINLOCK(rx_sleepLock); \
-        if ((code = kwakeup_one(KERNEL_ADDRESS, (cv), WAKEUP_ONE, &wo)) < 0) \
-            osi_Panic("kwakeup_one failed: code = %d \n", code); \
-        MP_SPINUNLOCK(rx_sleepLock); \
-    } while (0)
-
-/* Wakes up all threads waiting on this condition */
-#define CV_BROADCAST(cv) \
-    do { \
-        int wo, code; \
-        MP_SPINLOCK(rx_sleepLock); \
-        if ((code = kwakeup_one(KERNEL_ADDRESS, (cv), WAKEUP_ALL, &wo)) < 0) \
-            osi_Panic("kwakeup_all failed: code = %d \n", code); \
-        MP_SPINUNLOCK(rx_sleepLock); \
-    } while (0)
-#endif /* 0 */
-
 #define CV_DESTROY(a)
 
 /* We now use beta semaphores for mutexes */
