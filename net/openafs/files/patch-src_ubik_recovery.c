--- src/ubik/recovery.c.orig	2021-01-14 21:08:41 UTC
+++ src/ubik/recovery.c
@@ -465,8 +465,10 @@ urecovery_Interact(void *dummy)
     int fd = -1;
     afs_int32 pass;
 
-    afs_pthread_setname_self("recovery");
+    memset(pbuffer, 0, sizeof(pbuffer));
 
+    opr_threadname_set("recovery");
+
     /* otherwise, begin interaction */
     urecovery_state = 0;
     lastProbeTime = 0;
@@ -601,6 +603,8 @@ urecovery_Interact(void *dummy)
 	    /* we don't have the best version; we should fetch it. */
 	    urecovery_AbortAll(ubik_dbase);
 
+	    pbuffer[0] = '\0';
+
 	    /* Rx code to do the Bulk fetch */
 	    file = 0;
 	    offset = 0;
@@ -716,7 +720,9 @@ urecovery_Interact(void *dummy)
 #endif
 	    }
 	    if (code) {
-		unlink(pbuffer);
+		if (pbuffer[0] != '\0') {
+		    unlink(pbuffer);
+		}
 		/*
 		 * We will effectively invalidate the old data forever now.
 		 * Unclear if we *should* but we do.
