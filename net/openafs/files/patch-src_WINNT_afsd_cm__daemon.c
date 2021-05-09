--- src/WINNT/afsd/cm_daemon.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/cm_daemon.c
@@ -881,17 +881,6 @@ void cm_DaemonShutdown(void)
 
     if (cm_LockDaemon_ShutdownEvent)
         code = thrd_WaitForSingleObject_Event(cm_LockDaemon_ShutdownEvent, INFINITE);
-
-#if 0
-    /*
-     * Do not waste precious time waiting for the ipaddr daemon to shutdown.
-     * When it does it means we have lost our network connection and we need
-     * it during cache shutdown in order to notify the file servers that this
-     * client is giving up all callbacks.
-     */
-    if (cm_IPAddrDaemon_ShutdownEvent)
-        code = thrd_WaitForSingleObject_Event(cm_IPAddrDaemon_ShutdownEvent, INFINITE);
-#endif
 }
 
 void cm_InitDaemon(int nDaemons)
