--- src/afs/VNOPS/afs_vnop_lookup.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/VNOPS/afs_vnop_lookup.c
@@ -260,10 +260,6 @@ EvalMountPoint(struct vcache *avc, struct vcache *advc
     afs_uint32 avnoid, auniq;
 
     AFS_STATCNT(EvalMountPoint);
-#ifdef notdef
-    if (avc->mvid.target_root && (avc->f.states & CMValid))
-	return 0;		/* done while racing */
-#endif
     *avolpp = NULL;
     code = afs_HandleLink(avc, areq);
     if (code)
@@ -381,14 +377,14 @@ afs_EvalFakeStat_int(struct vcache **avcp, struct afs_
 
 	    do {
 		retry = 0;
-		ObtainWriteLock(&afs_xvcache, 597);
-		root_vp = afs_FindVCache(tvc->mvid.target_root, &retry, IS_WLOCK);
+		ObtainReadLock(&afs_xvcache);
+		root_vp = afs_FindVCache(tvc->mvid.target_root, &retry, 0);
 		if (root_vp && retry) {
-		    ReleaseWriteLock(&afs_xvcache);
+		    ReleaseReadLock(&afs_xvcache);
 		    afs_PutVCache(root_vp);
 		}
 	    } while (root_vp && retry);
-	    ReleaseWriteLock(&afs_xvcache);
+	    ReleaseReadLock(&afs_xvcache);
 	} else {
 	    root_vp = afs_GetVCache(tvc->mvid.target_root, areq, NULL, NULL);
 	}
@@ -695,6 +691,7 @@ afs_DoBulkStat(struct vcache *adp, long dirCookie, str
     struct VenusFid dotdot = {0, {0, 0, 0}};
     int flagIndex = 0;		/* First file with bulk fetch flag set */
     struct rx_connection *rxconn;
+    int attempt_i;
     XSTATS_DECLS;
     dotdot.Cell = 0;
     dotdot.Fid.Unique = 0;
@@ -788,7 +785,18 @@ afs_DoBulkStat(struct vcache *adp, long dirCookie, str
     /* now we have dir data in the cache, so scan the dir page */
     fidIndex = 0;
     flagIndex = 0;
-    while (1) {			/* Should probably have some constant bound */
+
+    /*
+     * Only examine at most the next 'nentries*4' entries to find dir entries
+     * to stat. This is an arbitrary limit that we set so we don't waste time
+     * scanning an entire dir that contains stat'd entries. For example, if a
+     * dir contains 10k entries, and all or almost all of them are stat'd, then
+     * we'll examine 10k entries for no benefit. For each entry, we run
+     * afs_FindVCache, and grab and release afs_xvcache; doing this e.g. 10k
+     * times can have significant impact if the client is under a lot of load.
+     */
+    for (attempt_i = 0; attempt_i < nentries * 4; attempt_i++) {
+
 	/* look for first safe entry to examine in the directory.  BlobScan
 	 * looks for a the 1st allocated dir after the dirCookie slot.
 	 */
@@ -825,14 +833,15 @@ afs_DoBulkStat(struct vcache *adp, long dirCookie, str
 	    tfid.Fid.Unique = ntohl(dirEntryp->fid.vunique);
 	    do {
 		retry = 0;
-		ObtainWriteLock(&afs_xvcache, 130);
-		tvcp = afs_FindVCache(&tfid, &retry, IS_WLOCK /* no stats | LRU */ );
+		ObtainSharedLock(&afs_xvcache, 130);
+		tvcp = afs_FindVCache(&tfid, &retry, IS_SLOCK /* no stats | LRU */ );
 		if (tvcp && retry) {
-		    ReleaseWriteLock(&afs_xvcache);
+		    ReleaseSharedLock(&afs_xvcache);
 		    afs_PutVCache(tvcp);
 		}
 	    } while (tvcp && retry);
 	    if (!tvcp) {	/* otherwise, create manually */
+		UpgradeSToWLock(&afs_xvcache, 129);
 		tvcp = afs_NewBulkVCache(&tfid, hostp, statSeqNo);
 		if (tvcp)
 		{
@@ -856,7 +865,7 @@ afs_DoBulkStat(struct vcache *adp, long dirCookie, str
 		    ReleaseWriteLock(&afs_xvcache);
 		}
 	    } else {
-		ReleaseWriteLock(&afs_xvcache);
+		ReleaseSharedLock(&afs_xvcache);
 	    }
 	    if (!tvcp)
 	    {
@@ -923,7 +932,7 @@ afs_DoBulkStat(struct vcache *adp, long dirCookie, str
 	if (temp <= 0)
 	    break;
 	dirCookie += temp;
-    }				/* while loop over all dir entries */
+    }				/* for loop over dir entries */
 
     /* now release the dir lock and prepare to make the bulk RPC */
     ReleaseReadLock(&dcp->lock);
@@ -1098,7 +1107,9 @@ afs_DoBulkStat(struct vcache *adp, long dirCookie, str
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
@@ -1323,13 +1334,49 @@ afs_DoBulkStat(struct vcache *adp, long dirCookie, str
     return code;
 }
 
-/* was: (AFS_DEC_ENV) || defined(AFS_OSF30_ENV) || defined(AFS_NCR_ENV) */
 #ifdef AFS_DARWIN80_ENV
 int AFSDOBULK = 0;
-#else
-static int AFSDOBULK = 1;
 #endif
 
+static int
+afs_ShouldTryBulkStat(struct vcache *adp)
+{
+#ifdef AFS_DARWIN80_ENV
+    if (!AFSDOBULK) {
+	return 0;
+    }
+#endif
+    if (AFS_IS_DISCONNECTED) {
+	/* We can't prefetch entries if we're offline. */
+	return 0;
+    }
+    if (adp->opens < 1) {
+	/* Don't bother prefetching entries if nobody is holding the dir open
+	 * while we're doing a lookup. */
+	return 0;
+    }
+    if (afs_VCacheStressed()) {
+	/* If we already have too many vcaches, don't create more vcaches we
+	 * may not even use. */
+	return 0;
+    }
+    if ((adp->f.states & CForeign)) {
+	/* Don't bulkstat for dfs xlator dirs. */
+	return 0;
+    }
+    if (afs_IsDynroot(adp)) {
+	/* Don't prefetch dynroot entries; that's pointless, since we generate
+	 * those locally. */
+	return 0;
+    }
+    if (afs_InReadDir(adp)) {
+	/* Don't bulkstat if we're in the middle of servicing a readdir() in
+	 * the same process. */
+	return 0;
+    }
+    return 1;
+}
+
 static_inline int
 osi_lookup_isdot(const char *aname)
 {
@@ -1477,7 +1524,11 @@ afs_lookup(OSI_VC_DECL(adp), char *aname, struct vcach
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
@@ -1499,7 +1550,11 @@ afs_lookup(OSI_VC_DECL(adp), char *aname, struct vcach
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
@@ -1725,9 +1780,7 @@ afs_lookup(OSI_VC_DECL(adp), char *aname, struct vcach
 	/* prefetch some entries, if the dir is currently open.  The variable
 	 * dirCookie tells us where to start prefetching from.
 	 */
-	if (!AFS_IS_DISCONNECTED && 
-	    AFSDOBULK && adp->opens > 0 && !(adp->f.states & CForeign)
-	    && !afs_IsDynroot(adp) && !afs_InReadDir(adp)) {
+	if (afs_ShouldTryBulkStat(adp)) {
 	    afs_int32 retry;
 	    /* if the entry is not in the cache, or is in the cache,
 	     * but hasn't been statd, then do a bulk stat operation.
