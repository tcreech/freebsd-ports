--- src/xstat/xstat_fs_test.c.orig	2021-01-14 21:08:41 UTC
+++ src/xstat/xstat_fs_test.c
@@ -778,8 +778,8 @@ RunTheTest(struct cmd_syndesc *a_s, void *dummy)
 	 */
 	if (debugging_on)
 	    printf("[%s] Calling LWP_WaitProcess() on event %" AFS_PTR_FMT "\n", rn,
-		   &terminationEvent);
-	waitCode = LWP_WaitProcess(&terminationEvent);
+		   &fs_terminationEvent);
+	waitCode = LWP_WaitProcess(&fs_terminationEvent);
 	if (debugging_on)
 	    printf("[%s] Returned from LWP_WaitProcess()\n", rn);
 	if (waitCode) {
