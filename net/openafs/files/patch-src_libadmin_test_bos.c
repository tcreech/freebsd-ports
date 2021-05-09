--- src/libadmin/test/bos.c.orig	2021-01-14 21:08:41 UTC
+++ src/libadmin/test/bos.c
@@ -1789,22 +1789,6 @@ DoBosSalvage(struct cmd_syndesc *as, void *arock)
     return 0;
 }
 
-#if 0
-static void
-Print_afs_RPCStatsState_p(afs_RPCStatsState_p state, const char *prefix)
-{
-    printf("%sThe rpc stats state is: ", prefix);
-    switch (*state) {
-    case AFS_RPC_STATS_DISABLED:
-	printf("disabled\n");
-	break;
-    case AFS_RPC_STATS_ENABLED:
-	printf("enabled\n");
-	break;
-    }
-}
-#endif
-
 void
 SetupBosAdminCmd(void)
 {
