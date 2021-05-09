--- src/vol/vol-bless.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/vol-bless.c
@@ -15,6 +15,7 @@
 #include <afs/cmd.h>
 
 #include <rx/xdr.h>
+#include <rx/rx_queue.h>
 #include <afs/afsint.h>
 #include "nfs.h"
 #include "lock.h"
