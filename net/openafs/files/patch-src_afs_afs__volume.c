--- src/afs/afs_volume.c.orig	2021-07-29 10:24:31 UTC
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
 
