--- src/viced/viced.c.orig	2021-01-14 21:08:41 UTC
+++ src/viced/viced.c
@@ -378,7 +378,7 @@ setThreadId(char *s)
 
     /* set our 'thread-id' so that the host hold table works */
     threadId = rx_SetThreadNum();
-    afs_pthread_setname_self(s);
+    opr_threadname_set(s);
     ViceLog(0, ("Set thread id 0x%x for '%s'\n", threadId, s));
 #endif
 }
@@ -683,7 +683,7 @@ PrintCounters(void)
 static void *
 ShutdownWatchdogLWP(void *unused)
 {
-    afs_pthread_setname_self("ShutdownWatchdog");
+    opr_threadname_set("ShutdownWatchdog");
     sleep(panic_timeout);
     ViceLogThenPanic(0, ("ShutdownWatchdogLWP: Failed to shutdown and panic "
                          "within %d seconds; forcing panic\n",
@@ -865,7 +865,6 @@ ParseRights(char *arights)
  * AIX:         sysconf() limit is real
  * HP-UX:       sysconf() limit is real
  * IRIX:        sysconf() limit is apparently NOT real -- too small
- * DUX:         sysconf() limit is apparently NOT real -- too big
  * Linux:       sysconf() limit is apparently NOT real -- too big
  * Solaris:     no sysconf() limit
  */
