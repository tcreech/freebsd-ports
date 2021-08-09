--- src/afs/VNOPS/afs_vnop_lookup.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/VNOPS/afs_vnop_lookup.c
@@ -1107,7 +1107,9 @@ afs_DoBulkStat(struct vcache *adp, long dirCookie, str
 	retry = 1;
     }
 #else
-    osi_vnhold(lruvcp, &retry);
+    if (osi_vnhold(lruvcp) != 0) {
+	retry = 1;
+    }
 #endif
     ReleaseReadLock(&afs_xvcache);	/* could be read lock */
     if (retry)
@@ -1522,7 +1524,11 @@ afs_lookup(OSI_VC_DECL(adp), char *aname, struct vcach
      */
     if (osi_lookup_isdot(aname)) {	/* special case */
 	ObtainReadLock(&afs_xvcache);
-	osi_vnhold(adp, 0);
+	if (osi_vnhold(adp) != 0) {
+	    ReleaseReadLock(&afs_xvcache);
+	    code = EIO;
+	    goto done;
+	}
 	ReleaseReadLock(&afs_xvcache);
 #ifdef AFS_DARWIN80_ENV
         vnode_get(AFSTOV(adp));
@@ -1544,7 +1550,11 @@ afs_lookup(OSI_VC_DECL(adp), char *aname, struct vcach
 	aname[0] == '.' && aname[1] == '.' && !aname[2]) {
 
 	ObtainReadLock(&afs_xvcache);
-	osi_vnhold(afs_globalVp, 0);
+	if (osi_vnhold(afs_globalVp) != 0) {
+	    ReleaseReadLock(&afs_xvcache);
+	    code = EIO;
+	    goto done;
+	}
 	ReleaseReadLock(&afs_xvcache);
 #ifdef AFS_DARWIN80_ENV
         vnode_get(AFSTOV(afs_globalVp));
