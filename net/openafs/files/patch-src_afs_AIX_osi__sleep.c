--- src/afs/AIX/osi_sleep.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/AIX/osi_sleep.c
@@ -20,11 +20,6 @@ static char waitV;
 static void
 AfsWaitHack(struct trb *trb)
 {
-#if 0
-/* this gets called at interrupt context; let's not tempt fate... */
-    AFS_STATCNT(WaitHack);
-#endif
-
     e_clear_wait(trb->func_data, THREAD_TIMED_OUT);
 }
 
