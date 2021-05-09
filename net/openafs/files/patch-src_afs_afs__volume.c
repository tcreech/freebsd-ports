--- src/afs/afs_volume.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_volume.c
@@ -548,7 +548,9 @@ loop:
 			continue;
 		    }
 #else
-		    AFS_FAST_HOLD(tvc);
+		    if (osi_vnhold(tvc) != 0) {
+			continue;
+		    }
 #endif
 		    ReleaseReadLock(&afs_xvcache);
 
@@ -910,25 +912,6 @@ afs_NewVolumeByName(char *aname, afs_int32 acell, int 
 			 SHARED_LOCK, tcell));
 
     if (code) {
-	/* If the client has yet to contact this cell and contact failed due
-	 * to network errors, mark the VLDB servers as back up.
-	 * That the client tried and failed can be determined from the
-	 * fact that there was a downtime incident, but CHasVolRef is not set.
-	 */
-    /* RT 48959 - unclear if this should really go */
-#if 0
-	if (areq->networkError && !(tcell->states & CHasVolRef)) {
-	    int i;
-	    struct server *sp;
-	    struct srvAddr *sap;
-	    for (i = 0; i < AFS_MAXCELLHOSTS; i++) {
-		if ((sp = tcell->cellHosts[i]) == NULL)
-		    break;
-		for (sap = sp->addr; sap; sap = sap->next_sa)
-		    afs_MarkServerUpOrDown(sap, 0);
-	    }
-	}
-#endif
 	afs_CopyError(treq, areq);
 	osi_FreeLargeSpace(tbuffer);
 	afs_PutCell(tcell, READ_LOCK);
