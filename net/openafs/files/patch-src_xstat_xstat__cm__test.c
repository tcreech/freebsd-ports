--- src/xstat/xstat_cm_test.c.orig	2021-01-14 21:08:41 UTC
+++ src/xstat/xstat_cm_test.c
@@ -881,8 +881,8 @@ RunTheTest(struct cmd_syndesc *a_s, void *arock)
 	 */
 	if (debugging_on)
 	    printf("[%s] Calling LWP_WaitProcess() on event %" AFS_PTR_FMT
-		   "\n", rn, &terminationEvent);
-	waitCode = LWP_WaitProcess(&terminationEvent);
+		   "\n", rn, &cm_terminationEvent);
+	waitCode = LWP_WaitProcess(&cm_terminationEvent);
 	if (debugging_on)
 	    printf("[%s] Returned from LWP_WaitProcess()\n", rn);
 	if (waitCode) {
