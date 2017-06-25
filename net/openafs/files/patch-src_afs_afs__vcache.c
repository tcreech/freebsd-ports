--- src/afs/afs_vcache.c.orig	2017-06-24 14:41:09 UTC
+++ src/afs/afs_vcache.c
@@ -1763,12 +1763,7 @@ afs_GetVCache(struct VenusFid *afid, str
 	iheldthelock = VOP_ISLOCKED(vp);
 	if (!iheldthelock) {
 	    /* nosleep/sleep lock order reversal */
-	    int glocked = ISAFS_GLOCK();
-	    if (glocked)
-		AFS_GUNLOCK();
 	    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY);
-	    if (glocked)
-		AFS_GLOCK();
 	}
 	vinvalbuf(vp, V_SAVE, PINOD, 0); /* changed late in 8.0-CURRENT */
 	if (!iheldthelock)
