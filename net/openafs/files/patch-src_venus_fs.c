--- src/venus/fs.c.orig	2021-01-14 21:08:41 UTC
+++ src/venus/fs.c
@@ -2988,10 +2988,6 @@ GetCellCmd(struct cmd_syndesc *as, void *arock)
 	    continue;
 	}
 	printf("Cell %s status: ", info.name);
-#ifdef notdef
-	if (args.stat & 1)
-	    printf("primary ");
-#endif
 	if (args.stat & 2)
 	    printf("no setuid allowed");
 	else
@@ -3736,11 +3732,6 @@ main(int argc, char **argv)
     cmd_AddParm(ts, "-path", CMD_LIST, CMD_OPTIONAL, "dir/file path");
     cmd_AddParm(ts, "-max", CMD_SINGLE, CMD_OPTIONAL,
 		"disk space quota in 1K units");
-#ifdef notdef
-    cmd_AddParm(ts, "-min", CMD_SINGLE, CMD_OPTIONAL,
-		"disk space guaranteed");
-    cmd_AddParm(ts, "-motd", CMD_SINGLE, CMD_OPTIONAL, "message of the day");
-#endif
     cmd_AddParm(ts, "-offlinemsg", CMD_SINGLE, CMD_OPTIONAL,
 		"offline message");
     cmd_CreateAlias(ts, "sv");
@@ -3835,9 +3826,6 @@ defect 3069
     ts = cmd_CreateSyntax("setquota", SetQuotaCmd, NULL, 0, "set volume quota");
     cmd_AddParm(ts, "-path", CMD_SINGLE, CMD_OPTIONAL, "dir/file path");
     cmd_AddParm(ts, "-max", CMD_SINGLE, 0, "max quota in kbytes");
-#ifdef notdef
-    cmd_AddParm(ts, "-min", CMD_SINGLE, CMD_OPTIONAL, "min quota in kbytes");
-#endif
     cmd_CreateAlias(ts, "sq");
 
     ts = cmd_CreateSyntax("newcell", NewCellCmd, NULL, 0, "configure new cell");
