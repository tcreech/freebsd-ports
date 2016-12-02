--- src/afs/FBSD/osi_sleep.c.orig	2016-11-10 16:56:30 UTC
+++ src/afs/FBSD/osi_sleep.c
@@ -16,6 +16,8 @@
 #include "afsincludes.h"	/* Afs-based standard headers */
 #include "afs/afs_stats.h"	/* afs statistics */
 
+static struct afs_osi_WaitHandle waitH;
+
 void
 afs_osi_InitWaitHandle(struct afs_osi_WaitHandle *achandle)
 {
@@ -34,12 +36,17 @@ void
 afs_osi_CancelWait(struct afs_osi_WaitHandle *achandle)
 {
     AFS_STATCNT(osi_CancelWait);
+    struct afs_osi_WaitHandle *chandle;
+
+    /* Assume our private/static handle if none provided. I don't think this
+     * happens right now. */
+    chandle = achandle == NULL ? &waitH : achandle;
 
     /* XXX should not be necessary */
-    if (!achandle->wh_inited)
-	return;
+    if (!chandle->wh_inited)
+        return;
     AFS_ASSERT_GLOCK();
-    cv_signal(&achandle->wh_condvar);
+    cv_signal(&chandle->wh_condvar);
 }
 
 /* afs_osi_Wait
@@ -47,11 +54,12 @@ afs_osi_CancelWait(struct afs_osi_WaitHa
  * Returns 0 if timeout and EINTR if signalled.
  */
 int
-afs_osi_Wait(afs_int32 ams, struct afs_osi_WaitHandle *ahandle, int aintok)
+afs_osi_Wait(afs_int32 ams, struct afs_osi_WaitHandle *arghandle, int aintok)
 {
     int code;
     struct timeval tv;
     int ticks;
+    struct afs_osi_WaitHandle *ahandle;
 
     AFS_STATCNT(osi_Wait);
     tv.tv_sec = ams / 1000;
@@ -59,19 +67,26 @@ afs_osi_Wait(afs_int32 ams, struct afs_o
     ticks = tvtohz(&tv);
 
     AFS_ASSERT_GLOCK();
-    if (ahandle == NULL) {
-	/* This is nasty and evil and rude. */
-	code = msleep(&tv, &afs_global_mtx, (aintok ? PPAUSE|PCATCH : PVFS),
-	    "afswait", ticks);
-    } else {
-	if (!ahandle->wh_inited)
-	    afs_osi_InitWaitHandle(ahandle);	/* XXX should not be needed */
-	if (aintok)
-	    code = cv_timedwait_sig(&ahandle->wh_condvar, &afs_global_mtx,
-		ticks);
-	else
-	    code = cv_timedwait(&ahandle->wh_condvar, &afs_global_mtx, ticks);
-    }
+
+    /* Note that we seem to have bypassed afs_osi_TimedSleep in FBSD -- it's
+     * used in afs_osi_Wait on other ports, but not in FBSD. Nor is it used
+     * anywhere else in FBSD.
+    */
+
+    /* This is perhaps similar to "waitV" in other ports. */
+    ahandle = arghandle == NULL ? &waitH : arghandle;
+
+    /* This will be used particularly for waitH, which as a global is initially
+     * all 0.  */
+    if (!ahandle->wh_inited)
+        afs_osi_InitWaitHandle(ahandle);
+
+    if (aintok)
+        code = cv_timedwait_sig(&ahandle->wh_condvar, &afs_global_mtx,
+                ticks);
+    else
+        code = cv_timedwait(&ahandle->wh_condvar, &afs_global_mtx, ticks);
+
     return code;
 }
 
@@ -175,12 +190,13 @@ afs_osi_TimedSleep(void *event, afs_int3
     evp = afs_getevent(event);
     seq = evp->seq;
     while (seq == evp->seq) {
-	AFS_ASSERT_GLOCK();
-	code = msleep(event, &afs_global_mtx, (aintok ? PPAUSE|PCATCH : PVFS),
-	    "afstslp", ticks);
-	if (code == EINTR)
-	    break;
+        AFS_ASSERT_GLOCK();
+        code = msleep(event, &afs_global_mtx, (aintok ? PPAUSE|PCATCH : PVFS),
+                "afstslp", ticks);
+        if (code == EINTR)
+            break;
     }
+
     relevent(evp);
     return code;
 }
