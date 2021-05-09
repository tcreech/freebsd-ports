--- tests/common/servers.c.orig	2021-01-14 21:08:41 UTC
+++ tests/common/servers.c
@@ -73,6 +73,7 @@ afstest_StopServer(pid_t serverPid)
 
 int
 afstest_StartTestRPCService(const char *configPath,
+			    pid_t signal_pid,
 			    u_short port,
 			    u_short serviceId,
 			    afs_int32 (*proc) (struct rx_call *))
@@ -93,6 +94,10 @@ afstest_StartTestRPCService(const char *configPath,
     if (code != 0) {
 	fprintf(stderr, "Server: Unable to initialise RX\n");
 	return -1;
+    }
+
+    if (signal_pid != 0) {
+	kill(signal_pid, SIGUSR1);
     }
 
     afsconf_BuildServerSecurityObjects(dir, &classes, &numClasses);
