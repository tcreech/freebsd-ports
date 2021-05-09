--- src/rxgen/rpc_parse.c.orig	2021-01-14 21:08:41 UTC
+++ src/rxgen/rpc_parse.c
@@ -1768,9 +1768,6 @@ ucs_ProcTail_setup(definition * defp, int split_flag)
 
     f_print(fout, "{\tafs_int32 rcode, code, newHost, thisHost, i, _ucount;\n");
     f_print(fout, "\tint chaseCount, pass, needsync;\n");
-#if 0 /* goes with block below */
-    f_print(fout, "\tint j, inlist;\n");
-#endif
     f_print(fout, "\tstruct rx_connection *tc;\n");
     f_print(fout, "\tstruct rx_peer *rxp;\n");
     f_print(fout, "\tshort origLevel;\n\n");
@@ -1781,17 +1778,6 @@ ucs_ProcTail_setup(definition * defp, int split_flag)
     f_print(fout, "\torigLevel = aclient->initializationState;\n");
     f_print(fout, "\trcode = UNOSERVERS;\n");
     f_print(fout, "\tchaseCount = needsync = 0;\n\n");
-#if 0 /* We should do some sort of caching algorithm for this, but I need to think about it - shadow 26 jun 06 */
-    f_print(fout, "\tinlist = 0;\n");
-    f_print(fout, "\tLOCK_UCLNT_CACHE;\n");
-    f_print(fout, "\tfor (j = 0; ((j < SYNCCOUNT) && calls_needsync[j]); j++) {\n");
-    f_print(fout, "\t\tif (calls_needsync[j] == (int *)%s%s%s) {\n", prefix, PackagePrefix[PackageIndex], defp->pc.proc_name);
-    f_print(fout, "\t\t\tinlist = needsync = 1;\n");
-    f_print(fout, "\t\t\tbreak;\n");
-    f_print(fout, "\t\t}\n");
-    f_print(fout, "\t}\n");
-    f_print(fout, "\tUNLOCK_UCLNT_CACHE;\n");
-#endif
     f_print(fout, "\t/* \n\t* First  pass, we try all servers that are up.\n\t* Second pass, we try all servers.\n\t*/\n");
     f_print(fout, "\tfor (pass = 0; pass < 2; pass++) {  /*p */\n");
     f_print(fout, "\t\t/* For each entry in our servers list */\n");
@@ -1878,15 +1864,6 @@ ucs_ProcTail_setup(definition * defp, int split_flag)
     f_print(fout, "\tdone:\n");
     f_print(fout, "\tif (needsync) {\n");
 
-#if 0 /* We should do some sort of caching algorithm for this, but I need to think about it - shadow 26 jun 06 */
-    f_print(fout, "\t\tif (!inlist) {          /* Remember proc call that needs sync site */\n");
-    f_print(fout, "\t\t\tLOCK_UCLNT_CACHE;\n");
-    f_print(fout, "\t\t\tcalls_needsync[synccount % SYNCCOUNT] = (int *)%s%s%s;\n", prefix, PackagePrefix[PackageIndex], defp->pc.proc_name);
-    f_print(fout, "\t\t\tsynccount++;\n");
-    f_print(fout, "\t\t\tUNLOCK_UCLNT_CACHE;\n");
-    f_print(fout, "\t\t\tinlist = 1;\n");
-    f_print(fout, "\t\t}\n");
-#endif
     f_print(fout, "\t\tif (!rcode) {           /* Remember the sync site - cmd successful */\n");
     f_print(fout, "\t\t\trxp = rx_PeerOf(aclient->conns[_ucount]);\n");
     f_print(fout, "\t\t\taclient->syncSite = rx_HostOf(rxp);\n");
