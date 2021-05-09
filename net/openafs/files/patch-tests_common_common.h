--- tests/common/common.h.orig	2021-01-14 21:08:41 UTC
+++ tests/common/common.h
@@ -40,7 +40,7 @@ extern struct rx_securityClass
 struct rx_call;
 extern int afstest_StartVLServer(char *dirname, pid_t *serverPid);
 extern int afstest_StopServer(pid_t serverPid);
-extern int afstest_StartTestRPCService(const char *, u_short, u_short,
+extern int afstest_StartTestRPCService(const char *, pid_t, u_short, u_short,
 				       afs_int32 (*proc)(struct rx_call *));
 
 /* ubik.c */
@@ -55,3 +55,6 @@ extern int afstest_GetUbikClient(struct afsconf_dir *d
 extern int afstest_IsLoopbackNetworkDefault(void);
 extern int afstest_SkipTestsIfLoopbackNetIsDefault(void);
 extern void afstest_SkipTestsIfBadHostname(void);
+
+/* misc.c */
+extern char *afstest_GetProgname(char **argv);
