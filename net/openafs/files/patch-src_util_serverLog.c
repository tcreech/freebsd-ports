--- src/util/serverLog.c.orig	2021-01-14 21:08:41 UTC
+++ src/util/serverLog.c
@@ -509,9 +509,24 @@ SetupLogSignals(void)
 
 #if defined(AFS_PTHREAD_ENV)
 static void
+LockServerLog(void)
+{
+    LOCK_SERVERLOG();
+}
+
+static void
+UnlockServerLog(void)
+{
+    UNLOCK_SERVERLOG();
+}
+
+static void
 InitServerLogMutex(void)
 {
     opr_Verify(pthread_mutex_init(&serverLogMutex, NULL) == 0);
+# ifndef AFS_NT40_ENV
+    opr_Verify(pthread_atfork(LockServerLog, UnlockServerLog, UnlockServerLog) == 0);
+# endif
 }
 #endif /* AFS_PTHREAD_ENV */
 
