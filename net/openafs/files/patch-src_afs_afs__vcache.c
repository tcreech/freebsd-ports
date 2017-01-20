--- src/afs/afs_vcache.c.orig	2016-12-06 16:17:37 UTC
+++ src/afs/afs_vcache.c
@@ -1764,7 +1764,13 @@ afs_GetVCache(struct VenusFid *afid, str
 	    if (glocked)
 		AFS_GLOCK();
 	}
+    int glocked_again = ISAFS_GLOCK();
+    // vinvalbuf can sleep; don't hold the lock.
+    if (glocked_again)
+        AFS_GUNLOCK();
 	vinvalbuf(vp, V_SAVE, PINOD, 0); /* changed late in 8.0-CURRENT */
+    if (glocked_again)
+        AFS_GLOCK();
 	if (!iheldthelock)
 	    VOP_UNLOCK(vp, 0);
 #elif defined(AFS_FBSD60_ENV)
