--- src/budb/dbs_dump.c.orig	2021-01-14 21:08:41 UTC
+++ src/budb/dbs_dump.c
@@ -61,7 +61,7 @@ setupDbDump(void *param)
     int writeFid = (intptr_t)param;
     afs_int32 code = 0;
 
-    afs_pthread_setname_self("Database Dumper");
+    opr_threadname_set("Database Dumper");
     code = InitRPC(&dumpSyncPtr->ut, LOCKREAD, 1);
     if (code)
 	goto error_exit;
@@ -327,7 +327,7 @@ dumpWatcher(void *unused)
 {
     afs_int32 code;
 
-    afs_pthread_setname_self("Database Dump Watchdog");
+    opr_threadname_set("Database Dump Watchdog");
     while (1) {			/*w */
 
 	/* printf("dumpWatcher\n"); */
