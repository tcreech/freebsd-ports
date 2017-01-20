--- src/afs/FBSD/osi_sleep.c.orig	2016-11-30 20:06:42 UTC
+++ src/afs/FBSD/osi_sleep.c
@@ -16,12 +16,13 @@
 #include "afsincludes.h"	/* Afs-based standard headers */
 #include "afs/afs_stats.h"	/* afs statistics */
 
+static char waitV;
+
 void
 afs_osi_InitWaitHandle(struct afs_osi_WaitHandle *achandle)
 {
     AFS_STATCNT(osi_InitWaitHandle);
-    cv_init(&achandle->wh_condvar, "afscondvar");
-    achandle->wh_inited = 1;
+    achandle->proc = (caddr_t) 0;
 }
 
 /* cancel osi_Wait */
@@ -33,13 +34,14 @@ afs_osi_InitWaitHandle(struct afs_osi_Wa
 void
 afs_osi_CancelWait(struct afs_osi_WaitHandle *achandle)
 {
-    AFS_STATCNT(osi_CancelWait);
+    caddr_t proc;
 
-    /* XXX should not be necessary */
-    if (!achandle->wh_inited)
-	return;
-    AFS_ASSERT_GLOCK();
-    cv_signal(&achandle->wh_condvar);
+    AFS_STATCNT(osi_CancelWait);
+    proc = achandle->proc;
+    if (proc == 0)
+        return;
+    achandle->proc = (caddr_t) 0;	/* so dude can figure out he was signalled */
+    afs_osi_Wakeup(&waitV);
 }
 
 /* afs_osi_Wait
@@ -50,28 +52,25 @@ int
 afs_osi_Wait(afs_int32 ams, struct afs_osi_WaitHandle *ahandle, int aintok)
 {
     int code;
-    struct timeval tv;
-    int ticks;
+    afs_int32 endTime;
 
     AFS_STATCNT(osi_Wait);
-    tv.tv_sec = ams / 1000;
-    tv.tv_usec = (ams % 1000) * 1000;
-    ticks = tvtohz(&tv);
+    endTime = osi_Time() + (ams / 1000);
+    if (ahandle)
+        ahandle->proc = (caddr_t) curthread;
+    do {
+        AFS_ASSERT_GLOCK();
+        code = 0;
+        code = afs_osi_TimedSleep(&waitV, ams, aintok);
 
-    AFS_ASSERT_GLOCK();
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
+        if (code)
+            break;		/* if something happened, quit now */
+        /* if we we're cancelled, quit now */
+        if (ahandle && (ahandle->proc == (caddr_t) 0)) {
+            /* we've been signalled */
+            break;
+        }
+    } while (osi_Time() < endTime);
     return code;
 }
 
@@ -85,7 +84,7 @@ typedef struct afs_event {
     int seq;			/* Sequence number: this is incremented
 				 * by wakeup calls; wait will not return until
 				 * it changes */
-    int cond;
+    struct cv cond;     /* Currently associated condition variable */
 } afs_event_t;
 
 #define HASHSIZE 128
@@ -118,6 +117,10 @@ afs_getevent(char *event)
 	afs_evhashcnt++;
 	newp->next = afs_evhasht[hashcode];
 	afs_evhasht[hashcode] = newp;
+    char *cv_name = (char*)afs_osi_Alloc_NoSleep(10);
+    static int cv_count = 0;
+    snprintf(cv_name, 9, "CV %04d", cv_count++);
+    cv_init(&newp->cond, event == &waitV ? "waitV" : cv_name);
 	newp->seq = 0;
     }
     newp->event = event;
@@ -139,7 +142,7 @@ afs_osi_Sleep(void *event)
     seq = evp->seq;
     while (seq == evp->seq) {
 	AFS_ASSERT_GLOCK();
-	msleep(event, &afs_global_mtx, PVFS, "afsslp", 0);
+    cv_wait(&evp->cond, &afs_global_mtx);
     }
     relevent(evp);
 }
@@ -173,14 +176,15 @@ afs_osi_TimedSleep(void *event, afs_int3
     ticks = tvtohz(&tv);
 
     evp = afs_getevent(event);
-    seq = evp->seq;
-    while (seq == evp->seq) {
-	AFS_ASSERT_GLOCK();
-	code = msleep(event, &afs_global_mtx, (aintok ? PPAUSE|PCATCH : PVFS),
-	    "afstslp", ticks);
-	if (code == EINTR)
-	    break;
+
+    AFS_ASSERT_GLOCK();
+    if (aintok) {
+        if (cv_timedwait_sig(&evp->cond, &afs_global_mtx, ticks) == 0)
+            code = EINTR;
+    } else {
+        cv_timedwait(&evp->cond, &afs_global_mtx, ticks);
     }
+
     relevent(evp);
     return code;
 }
@@ -194,7 +198,7 @@ afs_osi_Wakeup(void *event)
     evp = afs_getevent(event);
     if (evp->refcount > 1) {
 	evp->seq++;
-	wakeup(event);
+	cv_broadcast(&evp->cond);
 	ret = 0;
     }
     relevent(evp);
