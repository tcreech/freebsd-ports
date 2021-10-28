--- src/afs/afs_osidnlc.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/afs_osidnlc.c
@@ -275,7 +275,11 @@ osi_dnlc_lookup(struct vcache *adp, char *aname, int l
 	    return 0;
 	}
 #else
-	osi_vnhold(tvc, 0);
+	if (osi_vnhold(tvc) != 0) {
+	    dnlcstats.misses++;
+	    osi_dnlc_remove(adp, aname, tvc);
+	    tvc = NULL;
+	}
 #endif
 	ReleaseReadLock(&afs_xvcache);
 
