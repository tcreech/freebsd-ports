--- src/libadmin/samples/rxstat_get_process.c.orig	2021-01-14 21:08:41 UTC
+++ src/libadmin/samples/rxstat_get_process.c
@@ -110,7 +110,7 @@ GetPrintStrings(afs_RPCStats_p statp, char *ifName, ch
 	*funcListLen = VOTE_NO_OF_STAT_FUNCS;
 	break;
     case VL_STATINDEX:
-	strcpy(ifName, "volserver interface");
+	strcpy(ifName, "vlserver interface");
 	*funcList = VL_function_names;
 	*funcListLen = VL_NO_OF_STAT_FUNCS;
 	break;
