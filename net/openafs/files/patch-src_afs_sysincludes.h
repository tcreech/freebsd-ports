--- src/afs/sysincludes.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/sysincludes.h
@@ -212,6 +212,7 @@ typedef unsigned short etap_event_t;
 #  include <sys/socket.h>
 #  include <net/if.h>
 #  include <net/if_var.h>
+#  include <sys/rmlock.h>
 # endif
 
 # ifdef	AFS_AIX_ENV
