--- src/afs/afs_vcache.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/afs_vcache.c
@@ -1081,7 +1081,7 @@ afs_NewVCache_int(struct VenusFid *afid, struct server
 
 #if defined(AFS_LINUX22_ENV)
     /* Hold it for the LRU (should make count 2) */
-    AFS_FAST_HOLD(tvc);
+    osi_Assert(osi_vnhold(tvc) == 0);
 #elif !(defined (AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV))
     VREFCOUNT_SET(tvc, 1);	/* us */
 #endif
@@ -1145,8 +1145,10 @@ afs_FlushActiveVcaches(afs_int32 doflocks)
 #endif
 	    if (doflocks && tvc->flockCount != 0) {
 		struct rx_connection *rxconn;
+		if (osi_vnhold(tvc) != 0) {
+		    continue;
+		}
 		/* if this entry has an flock, send a keep-alive call out */
-		osi_vnhold(tvc, 0);
 		ReleaseReadLock(&afs_xvcache);
 		ObtainWriteLock(&tvc->lock, 51);
 		do {
@@ -1189,7 +1191,9 @@ afs_FlushActiveVcaches(afs_int32 doflocks)
 		 * this code.  Also, drop the afs_xvcache lock while
 		 * getting vcache locks.
 		 */
-		osi_vnhold(tvc, 0);
+		if (osi_vnhold(tvc) != 0) {
+		    continue;
+		}
 		ReleaseReadLock(&afs_xvcache);
 #if defined(AFS_SGI_ENV)
 		/*
@@ -1802,6 +1806,7 @@ afs_GetVCache(struct VenusFid *afid, struct vrequest *
      * is locked, and if it isn't, then we gain and drop it around the call
      * to vinvalbuf; otherwise, we leave it alone.
      */
+    ReleaseWriteLock(&tvc->lock);
     {
 	struct vnode *vp = AFSTOV(tvc);
 	int iheldthelock;
@@ -1810,30 +1815,28 @@ afs_GetVCache(struct VenusFid *afid, struct vrequest *
 	iheldthelock = VOP_ISLOCKED(vp);
 	if (!iheldthelock)
 	    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY, current_proc());
-	/* this is messy. we can call fsync which will try to reobtain this */
-	if (VTOAFS(vp) == tvc)
-	  ReleaseWriteLock(&tvc->lock);
 	if (UBCINFOEXISTS(vp)) {
 	  vinvalbuf(vp, V_SAVE, &afs_osi_cred, current_proc(), PINOD, 0);
 	}
-	if (VTOAFS(vp) == tvc)
-	  ObtainWriteLock(&tvc->lock, 954);
 	if (!iheldthelock)
 	    VOP_UNLOCK(vp, LK_EXCLUSIVE, current_proc());
 #elif defined(AFS_FBSD_ENV)
-	iheldthelock = VOP_ISLOCKED(vp);
+	AFS_GUNLOCK();
+	iheldthelock = 0;
+	if (VOP_ISLOCKED(vp) == LK_EXCLUSIVE) {
+	    iheldthelock = 1;
+	}
 	if (!iheldthelock) {
-	    /* nosleep/sleep lock order reversal */
-	    int glocked = ISAFS_GLOCK();
-	    if (glocked)
-		AFS_GUNLOCK();
 	    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY);
-	    if (glocked)
-		AFS_GLOCK();
 	}
 	vinvalbuf(vp, V_SAVE, PINOD, 0); /* changed late in 8.0-CURRENT */
 	if (!iheldthelock)
+#    if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	    VOP_UNLOCK(vp);
+#    else
 	    VOP_UNLOCK(vp, 0);
+#    endif
+	AFS_GLOCK();
 #elif defined(AFS_OBSD_ENV)
 	iheldthelock = VOP_ISLOCKED(vp, curproc);
 	if (!iheldthelock)
@@ -1851,6 +1854,11 @@ afs_GetVCache(struct VenusFid *afid, struct vrequest *
 	    VOP_UNLOCK(vp, 0);
 #endif
     }
+    ObtainWriteLock(&tvc->lock, 954);
+    if (tvc->f.states & CStatd) {
+	ReleaseWriteLock(&tvc->lock);
+	return tvc;
+    }
 #endif
 #endif
 
@@ -2183,6 +2191,10 @@ afs_GetRootVCache(struct VenusFid *afid, struct vreque
 		AFS_GLOCK();
 	        continue;
 	    }
+#else
+	    if (osi_vnhold(tvc) != 0) {
+		continue;
+	    }
 #endif
 	    break;
 	}
@@ -2191,14 +2203,19 @@ afs_GetRootVCache(struct VenusFid *afid, struct vreque
     if (!haveStatus && (!tvc || !(tvc->f.states & CStatd))) {
 	/* Mount point no longer stat'd or unknown. FID may have changed. */
 	getNewFid = 1;
-	ReleaseSharedLock(&afs_xvcache);
 #ifdef AFS_DARWIN80_ENV
+	ReleaseSharedLock(&afs_xvcache);
         if (tvc) {
             AFS_GUNLOCK();
             vnode_put(AFSTOV(tvc));
             vnode_rele(AFSTOV(tvc));
             AFS_GLOCK();
         }
+#else
+	if (tvc) {
+	    AFS_FAST_RELE(tvc);
+	}
+	ReleaseSharedLock(&afs_xvcache);
 #endif
         tvc = NULL;
 	goto newmtpt;
@@ -2219,11 +2236,6 @@ afs_GetRootVCache(struct VenusFid *afid, struct vreque
 	if (cached)
 	    *cached = 1;
 	afs_stats_cmperf.vcacheHits++;
-#if	defined(AFS_DARWIN80_ENV)
-	/* we already bumped the ref count in the for loop above */
-#else /* AFS_DARWIN80_ENV */
-	osi_vnhold(tvc, 0);
-#endif
 	UpgradeSToWLock(&afs_xvcache, 24);
 	if ((VLRU.next->prev != &VLRU) || (VLRU.prev->next != &VLRU)) {
 	    refpanic("GRVC VLRU inconsistent0");
@@ -2609,7 +2621,9 @@ afs_RefVCache(struct vcache *tvc)
 	return -1;
     }
 #else
-	osi_vnhold(tvc, 0);
+    if (osi_vnhold(tvc) != 0) {
+	return -1;
+    }
 #endif
     return 0;
 }				/*afs_RefVCache */
@@ -2661,8 +2675,6 @@ afs_FindVCache(struct VenusFid *afid, afs_int32 * retr
 
     /* should I have a read lock on the vnode here? */
     if (tvc) {
-	if (retry)
-	    *retry = 0;
 #if defined(AFS_DARWIN80_ENV)
 	tvp = AFSTOV(tvc);
 	if (vnode_get(tvp))
@@ -2688,10 +2700,12 @@ afs_FindVCache(struct VenusFid *afid, afs_int32 * retr
 	AFS_GLOCK();
 	tvc->f.states &= ~CUBCinit;
 #else
-	osi_vnhold(tvc, retry);	/* already held, above */
-	if (retry && *retry)
-	    return 0;
+	if (osi_vnhold(tvc) != 0) {
+	    tvc = NULL;
+	}
 #endif
+    }
+    if (tvc) {
 	/*
 	 * only move to front of vlru if we have proper vcache locking)
 	 */
@@ -2803,11 +2817,19 @@ afs_NFSFindVCache(struct vcache **avcp, struct VenusFi
 		AFS_GLOCK();
 		continue;
 	    }
+#else
+	    if (osi_vnhold(tvc) != 0) {
+		continue;
+	    }
 #endif /* AFS_DARWIN80_ENV */
 	    count++;
 	    if (found_tvc) {
 		/* Duplicates */
 		afs_duplicate_nfs_fids++;
+#ifndef AFS_DARWIN80_ENV
+		AFS_FAST_RELE(tvc);
+		AFS_FAST_RELE(found_tvc);
+#endif
 		ReleaseSharedLock(&afs_xvcache);
 #ifdef AFS_DARWIN80_ENV
                 /* Drop our reference counts. */
@@ -2823,21 +2845,6 @@ afs_NFSFindVCache(struct vcache **avcp, struct VenusFi
     tvc = found_tvc;
     /* should I have a read lock on the vnode here? */
     if (tvc) {
-#ifndef AFS_DARWIN80_ENV
-#if defined(AFS_SGI_ENV) && !defined(AFS_SGI53_ENV)
-	afs_int32 retry = 0;
-	osi_vnhold(tvc, &retry);
-	if (retry) {
-	    count = 0;
-	    found_tvc = (struct vcache *)0;
-	    ReleaseSharedLock(&afs_xvcache);
-	    spunlock_psema(tvc->v.v_lock, retry, &tvc->v.v_sync, PINOD);
-	    goto loop;
-	}
-#else
-	osi_vnhold(tvc, (int *)0);	/* already held, above */
-#endif
-#endif
 	/*
 	 * We obtained the xvcache lock above.
 	 */
@@ -2954,6 +2961,9 @@ shutdown_vcache(void)
 {
     int i;
     struct afs_cbr *tsp;
+    struct afs_cbr *cbr, *cbr_next;
+    struct server *ts;
+
     /*
      * XXX We may potentially miss some of the vcaches because if when
      * there are no free vcache entries and all the vcache entries are active
@@ -3023,6 +3033,20 @@ shutdown_vcache(void)
 	    afs_vhashT[i] = 0;
 	}
     }
+
+    /*
+     * Remove any reference to CBRs in the server structs before we free the
+     * memory for our CBRs below.
+     */
+    for (i = 0; i < NSERVERS; i++) {
+	for (ts = afs_servers[i]; ts; ts = ts->next) {
+	    for (cbr = ts->cbrs; cbr; cbr = cbr_next) {
+		cbr_next = cbr->next;
+		afs_FreeCBR(cbr);
+	    }
+	}
+    }
+
     /*
      * Free any leftover callback queue
      */
