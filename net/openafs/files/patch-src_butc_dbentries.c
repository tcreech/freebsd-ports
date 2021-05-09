--- src/butc/dbentries.c.orig	2021-01-14 21:08:41 UTC
+++ src/butc/dbentries.c
@@ -12,6 +12,7 @@
 
 #include <roken.h>
 
+#include <afs/opr.h>
 #include <rx/xdr.h>
 #include <rx/rx.h>
 #include <lwp.h>
@@ -31,7 +32,7 @@
 #include "error_macros.h"
 
 dlqlinkT savedEntries;
-dlqlinkT entries_to_flush;
+static dlqlinkT entries_to_flush;
 
 int dbWatcherinprogress;
 
@@ -328,7 +329,7 @@ dbWatcher(void *unused)
     afs_int32 code = 0;
     int i, c, addedDump;
 
-    afs_pthread_setname_self("dbWatcher");
+    opr_threadname_set("dbWatcher");
     dlqInit(&entries_to_flush);
     dlqInit(&savedEntries);
 
