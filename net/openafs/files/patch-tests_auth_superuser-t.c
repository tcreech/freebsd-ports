--- tests/auth/superuser-t.c.orig	2021-01-14 21:08:41 UTC
+++ tests/auth/superuser-t.c
@@ -29,6 +29,8 @@
 #ifdef HAVE_SYS_WAIT_H
 #include <sys/wait.h>
 #endif
+#include <signal.h>
+#include <time.h>
 
 #ifdef IGNORE_SOME_GCC_WARNINGS
 # pragma GCC diagnostic warning "-Wdeprecated-declarations"
@@ -365,6 +367,33 @@ STEST_NewWhoAmI(struct rx_call *call, char **result)
    return 0;
 }
 
+/*
+ * Primitive replacement for using sigtimedwait(). Just see if 'signo' is
+ * pending, and if it's not, wait 100ms and try again. Try this for
+ * approximately as many times as it takes to wait for 'nsecs' seconds.
+ */
+static int
+waitforsig(int signo, int nsecs)
+{
+    int nsleeps;
+
+    for (nsleeps = 0; nsleeps < nsecs * 10; nsleeps++) {
+	sigset_t set;
+	int code;
+
+	opr_Verify(sigpending(&set) == 0);
+	if (sigismember(&set, signo)) {
+	    return 0;
+	}
+
+	/* Sleep for 100ms */
+	code = usleep(100000);
+	opr_Assert(code == 0 || errno == EINTR);
+    }
+
+    return -1;
+}
+
 int main(int argc, char **argv)
 {
     struct afsconf_dir *dir;
@@ -372,6 +401,8 @@ int main(int argc, char **argv)
     int serverPid, clientPid, waited, stat;
     int code;
     int ret = 0;
+    sigset_t set;
+    char *argv0 = afstest_GetProgname(argv);
 
     afstest_SkipTestsIfBadHostname();
 
@@ -380,8 +411,8 @@ int main(int argc, char **argv)
     if (argc == 3 ) {
         if (strcmp(argv[1], "-server") == 0) {
 	    globalDir = afsconf_Open(argv[2]);
-	    afstest_StartTestRPCService(argv[2], TEST_PORT, TEST_SERVICE_ID,
-					TEST_ExecuteRequest);
+	    afstest_StartTestRPCService(argv[2], getppid(), TEST_PORT,
+					TEST_SERVICE_ID, TEST_ExecuteRequest);
             exit(0);
         } else if (strcmp(argv[1], "-client") == 0) {
             startClient(argv[2]);
@@ -395,6 +426,10 @@ int main(int argc, char **argv)
     /* Otherwise, do the basic configuration, then start the client and
      * server */
 
+    sigemptyset(&set);
+    sigaddset(&set, SIGUSR1);
+    opr_Verify(sigprocmask(SIG_BLOCK, &set, NULL) == 0);
+
     dirname = afstest_BuildTestConfig();
 
     dir = afsconf_Open(dirname);
@@ -406,7 +441,7 @@ int main(int argc, char **argv)
 
     code = afstest_AddDESKeyFile(dir);
     if (code) {
-	afs_com_err("superuser-t", code, "while adding new key\n");
+	afs_com_err(argv0, code, "while adding new key\n");
 	ret = 1;
 	goto out;
     }
@@ -420,6 +455,17 @@ int main(int argc, char **argv)
 	ret = 1;
 	goto out;
     }
+
+    /* Our server child pid will send us a SIGUSR1 when it's started listening
+     * on its port. Wait for up to 5 seconds to get the USR1. */
+    if (waitforsig(SIGUSR1, 5) != 0) {
+	fprintf(stderr, "%s: Timed out waiting for SIGUSR1 from server child\n",
+		argv0);
+	kill(serverPid, SIGTERM);
+	ret = 1;
+	goto out;
+    }
+
     clientPid = fork();
     if (clientPid == -1) {
         kill(serverPid, SIGTERM);
