--- src/ubik/beacon.c.orig	2021-01-14 21:08:41 UTC
+++ src/ubik/beacon.c
@@ -445,7 +445,7 @@ ubeacon_Interact(void *dummy)
     struct ubik_version tversion;
     afs_int32 startTime;
 
-    afs_pthread_setname_self("beacon");
+    opr_threadname_set("beacon");
 
     /* loop forever getting votes */
     lastWakeupTime = 0;		/* keep track of time we last started a vote collection */
