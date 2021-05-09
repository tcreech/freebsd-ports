--- src/afs/afs_vcache.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_vcache.c
@@ -747,7 +747,57 @@ afs_PostPopulateVCache(struct vcache *avc, struct Venu
     afs_osi_Wakeup(&avc->f.states);
 }
 
+/*
+ * afs_VCacheStressed() is intended to determine if the stat cache looks
+ * stressed / full-ish. Due to the different strategies of allocating vcaches
+ * on different platforms, the definition of "stressed" varies, and is somewhat
+ * arbitrary. We just try to make a reasonable guess here.
+ *
+ * Returns 1 if the stat cache looks stressed, and 0 otherwise.
+ */
+#ifdef AFS_LINUX26_ENV
 int
+afs_VCacheStressed(void)
+{
+    if (afsd_dynamic_vcaches) {
+	/*
+	 * For dynamic vcaches, the number of vcaches in use can vary wildly.
+	 * Consider us stressed if we're significantly above the configured
+	 * threshold. VCACHE_DYNAMIC_STRESSED is the arbitrary point at which
+	 * we're considered "significantly" over the threshold.
+	 */
+	if (afs_vcount > afs_cacheStats + VCACHE_DYNAMIC_STRESSED) {
+	    return 1;
+	}
+	return 0;
+
+    } else {
+	/*
+	 * For non-dynamic vcaches, we should never go above the configured
+	 * limit, and ShakeLooseVCaches should try to get us to VCACHE_FREE
+	 * under the limit. So if we're closer then VCACHE_FREE/2, then we're
+	 * very close to the limit, so consider us stressed.
+	 */
+	if (afs_vcount > afs_cacheStats || afs_cacheStats - afs_vcount < VCACHE_FREE/2) {
+	    return 1;
+	}
+	return 0;
+    }
+}
+#else /* AFS_LINUX26_ENV */
+int
+afs_VCacheStressed(void)
+{
+    /* If we don't have any vcaches in the free list, then consider the stat
+     * cache stressed. */
+    if (freeVCList != NULL) {
+	return 0;
+    }
+    return 1;
+}
+#endif /* AFS_LINUX26_ENV */
+
+int
 afs_ShakeLooseVCaches(afs_int32 anumber)
 {
     afs_int32 i, loop;
@@ -756,7 +806,6 @@ afs_ShakeLooseVCaches(afs_int32 anumber)
     struct afs_q *tq, *uq;
     int fv_slept, defersleep = 0;
     int limit;
-    afs_int32 target = anumber;
 
     loop = 0;
 
@@ -806,9 +855,27 @@ afs_ShakeLooseVCaches(afs_int32 anumber)
 	    break;
 	}
     }
-    if (!afsd_dynamic_vcaches && anumber == target) {
-	afs_warn("afs_ShakeLooseVCaches: warning none freed, using %d of %d\n",
-	       afs_vcount, afs_maxvcount);
+
+    if (afs_VCacheStressed()) {
+	/*
+	 * If it looks like we have too many vcaches, right after
+	 * ShakeLooseVCaches has tried to trim down the number of vcaches, then
+	 * maybe -stat should be increased. Log a warning, so if this is
+	 * causing problems the user has a chance at noticing.
+	 */
+	static afs_uint32 last_warned;
+	afs_uint32 now = osi_Time();
+
+	/* Warn about this at most once every VCACHE_STRESS_LOGINTERVAL secs */
+	if (now - last_warned > VCACHE_STRESS_LOGINTERVAL) {
+	    last_warned = now;
+	    afs_warn("afs: Warning: We are having trouble keeping the AFS stat "
+		     "cache trimmed down under the configured limit (current "
+		     "-stat setting: %d, current vcache usage: %d).\n",
+		     afs_cacheStats, afs_vcount);
+	    afs_warn("afs: If AFS access seems slow, consider raising the "
+		     "-stat setting for afsd.\n");
+	}
     }
 
     return 0;
@@ -1014,7 +1081,7 @@ afs_NewVCache_int(struct VenusFid *afid, struct server
 
 #if defined(AFS_LINUX22_ENV)
     /* Hold it for the LRU (should make count 2) */
-    AFS_FAST_HOLD(tvc);
+    osi_Assert(osi_vnhold(tvc) == 0);
 #elif !(defined (AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV))
     VREFCOUNT_SET(tvc, 1);	/* us */
 #endif
@@ -1078,8 +1145,10 @@ afs_FlushActiveVcaches(afs_int32 doflocks)
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
@@ -1122,7 +1191,9 @@ afs_FlushActiveVcaches(afs_int32 doflocks)
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
@@ -1452,18 +1523,6 @@ afs_WriteVCacheDiscon(struct vcache *avc,
 	if (astatus->Mask & AFS_SETMODE) {
 		avc->f.m.Mode = astatus->UnixModeBits;
 
-#if 0 	/* XXX: Leaving this out, so it doesn't mess up the file type flag.*/
-
-		if (vType(avc) == VREG) {
-			avc->f.m.Mode |= S_IFREG;
-		} else if (vType(avc) == VDIR) {
-			avc->f.m.Mode |= S_IFDIR;
-		} else if (vType(avc) == VLNK) {
-			avc->f.m.Mode |= S_IFLNK;
-			if ((avc->f.m.Mode & 0111) == 0)
-				avc->mvstat = AFS_MVSTAT_MTPT;
-		}
-#endif
 		flags |= VDisconSetMode;
 	 } 		/* if(astatus.Mask & AFS_SETMODE) */
 
@@ -1747,6 +1806,7 @@ afs_GetVCache(struct VenusFid *afid, struct vrequest *
      * is locked, and if it isn't, then we gain and drop it around the call
      * to vinvalbuf; otherwise, we leave it alone.
      */
+    ReleaseWriteLock(&tvc->lock);
     {
 	struct vnode *vp = AFSTOV(tvc);
 	int iheldthelock;
@@ -1755,46 +1815,28 @@ afs_GetVCache(struct VenusFid *afid, struct vrequest *
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
-#elif defined(AFS_FBSD80_ENV)
-	iheldthelock = VOP_ISLOCKED(vp);
+#elif defined(AFS_FBSD_ENV)
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
-#elif defined(AFS_FBSD60_ENV)
-	iheldthelock = VOP_ISLOCKED(vp, curthread);
-	if (!iheldthelock)
-	    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY, curthread);
-	AFS_GUNLOCK();
-	vinvalbuf(vp, V_SAVE, curthread, PINOD, 0);
+#    endif
 	AFS_GLOCK();
-	if (!iheldthelock)
-	    VOP_UNLOCK(vp, LK_EXCLUSIVE, curthread);
-#elif defined(AFS_FBSD_ENV)
-	iheldthelock = VOP_ISLOCKED(vp, curthread);
-	if (!iheldthelock)
-	    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY, curthread);
-	vinvalbuf(vp, V_SAVE, osi_curcred(), curthread, PINOD, 0);
-	if (!iheldthelock)
-	    VOP_UNLOCK(vp, LK_EXCLUSIVE, curthread);
 #elif defined(AFS_OBSD_ENV)
 	iheldthelock = VOP_ISLOCKED(vp, curproc);
 	if (!iheldthelock)
@@ -1812,6 +1854,11 @@ afs_GetVCache(struct VenusFid *afid, struct vrequest *
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
 
@@ -2144,6 +2191,10 @@ afs_GetRootVCache(struct VenusFid *afid, struct vreque
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
@@ -2152,14 +2203,19 @@ afs_GetRootVCache(struct VenusFid *afid, struct vreque
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
@@ -2180,11 +2236,6 @@ afs_GetRootVCache(struct VenusFid *afid, struct vreque
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
@@ -2450,145 +2501,6 @@ afs_FetchStatus(struct vcache * avc, struct VenusFid *
     return code;
 }
 
-#if 0
-/*
- * afs_StuffVcache
- *
- * Description:
- *	Stuff some information into the vcache for the given file.
- *
- * Parameters:
- *	afid	  : File in question.
- *	OutStatus : Fetch status on the file.
- *	CallBack  : Callback info.
- *	tc	  : RPC connection involved.
- *	areq	  : vrequest involved.
- *
- * Environment:
- *	Nothing interesting.
- */
-void
-afs_StuffVcache(struct VenusFid *afid,
-		struct AFSFetchStatus *OutStatus,
-		struct AFSCallBack *CallBack, struct afs_conn *tc,
-		struct vrequest *areq)
-{
-    afs_int32 code, i, newvcache = 0;
-    struct vcache *tvc;
-    struct AFSVolSync tsync;
-    struct volume *tvp;
-    struct axscache *ac;
-    afs_int32 retry;
-
-    AFS_STATCNT(afs_StuffVcache);
-#ifdef IFS_VCACHECOUNT
-    ifs_gvcachecall++;
-#endif
-
-  loop:
-    ObtainSharedLock(&afs_xvcache, 8);
-
-    tvc = afs_FindVCache(afid, &retry, DO_VLRU| IS_SLOCK /* no stats */ );
-    if (tvc && retry) {
-#if	defined(AFS_SGI_ENV) && !defined(AFS_SGI53_ENV)
-	ReleaseSharedLock(&afs_xvcache);
-	spunlock_psema(tvc->v.v_lock, retry, &tvc->v.v_sync, PINOD);
-	goto loop;
-#endif
-    }
-
-    if (!tvc) {
-	/* no cache entry, better grab one */
-	UpgradeSToWLock(&afs_xvcache, 25);
-	tvc = afs_NewVCache(afid, NULL);
-	newvcache = 1;
-	ConvertWToSLock(&afs_xvcache);
-	if (!tvc)
-	{
-		ReleaseSharedLock(&afs_xvcache);
-		return NULL;
-	}
-    }
-
-    ReleaseSharedLock(&afs_xvcache);
-    ObtainWriteLock(&tvc->lock, 58);
-
-    afs_StaleVCacheFlags(tvc, AFS_STALEVC_NOCB, 0);
-
-    /* Is it always appropriate to throw away all the access rights? */
-    afs_FreeAllAxs(&(tvc->Access));
-
-    /*Copy useful per-volume info */
-    tvp = afs_GetVolume(afid, areq, READ_LOCK);
-    if (tvp) {
-	if (newvcache && (tvp->states & VForeign))
-	    tvc->f.states |= CForeign;
-	if (tvp->states & VRO)
-	    tvc->f.states |= CRO;
-	if (tvp->states & VBackup)
-	    tvc->f.states |= CBackup;
-	/*
-	 * Now, copy ".." entry back out of volume structure, if
-	 * necessary
-	 */
-	if (tvc->mvstat == AFS_MVSTAT_ROOT && tvp->dotdot.Fid.Volume != 0) {
-	    if (!tvc->mvid.parent)
-		tvc->mvid.parent = (struct VenusFid *)
-		    osi_AllocSmallSpace(sizeof(struct VenusFid));
-	    *tvc->mvid.parent = tvp->dotdot;
-	}
-    }
-    /* store the stat on the file */
-    afs_RemoveVCB(afid);
-    afs_ProcessFS(tvc, OutStatus, areq);
-    tvc->callback = tc->srvr->server;
-
-    /* we use osi_Time twice below.  Ideally, we would use the time at which
-     * the FetchStatus call began, instead, but we don't have it here.  So we
-     * make do with "now".  In the CRO case, it doesn't really matter. In
-     * the other case, we hope that the difference between "now" and when the
-     * call actually began execution on the server won't be larger than the
-     * padding which the server keeps.  Subtract 1 second anyway, to be on
-     * the safe side.  Can't subtract more because we don't know how big
-     * ExpirationTime is.  Possible consistency problems may arise if the call
-     * timeout period becomes longer than the server's expiration padding.  */
-    ObtainWriteLock(&afs_xcbhash, 470);
-    if (CallBack->ExpirationTime != 0) {
-	tvc->cbExpires = CallBack->ExpirationTime + osi_Time() - 1;
-	tvc->f.states |= CStatd;
-	tvc->f.states &= ~CBulkFetching;
-	afs_QueueCallback(tvc, CBHash(CallBack->ExpirationTime), tvp);
-    } else if (tvc->f.states & CRO) {
-	/* old-fashioned AFS 3.2 style */
-	tvc->cbExpires = 3600 + osi_Time();
-	 /*XXX*/ tvc->f.states |= CStatd;
-	tvc->f.states &= ~CBulkFetching;
-	afs_QueueCallback(tvc, CBHash(3600), tvp);
-    } else {
-	afs_StaleVCacheFlags(tvc, AFS_STALEVC_CBLOCKED | AFS_STALEVC_CLEARCB,
-			     CUnique);
-    }
-    ReleaseWriteLock(&afs_xcbhash);
-    if (tvp)
-	afs_PutVolume(tvp, READ_LOCK);
-
-    /* look in per-pag cache */
-    if (tvc->Access && (ac = afs_FindAxs(tvc->Access, areq->uid)))
-	ac->axess = OutStatus->CallerAccess;	/* substitute pags */
-    else			/* not found, add a new one if possible */
-	afs_AddAxs(tvc->Access, areq->uid, OutStatus->CallerAccess);
-
-    ReleaseWriteLock(&tvc->lock);
-    afs_Trace4(afs_iclSetp, CM_TRACE_STUFFVCACHE, ICL_TYPE_POINTER, tvc,
-	       ICL_TYPE_POINTER, tvc->callback, ICL_TYPE_INT32,
-	       tvc->cbExpires, ICL_TYPE_INT32, tvc->cbExpires - osi_Time());
-    /*
-     * Release ref count... hope this guy stays around...
-     */
-    afs_PutVCache(tvc);
-}				/*afs_StuffVcache */
-#endif
-
 /*!
  * Decrements the reference count on a cache entry.
  *
@@ -2709,7 +2621,9 @@ afs_RefVCache(struct vcache *tvc)
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
@@ -2761,8 +2675,6 @@ afs_FindVCache(struct VenusFid *afid, afs_int32 * retr
 
     /* should I have a read lock on the vnode here? */
     if (tvc) {
-	if (retry)
-	    *retry = 0;
 #if defined(AFS_DARWIN80_ENV)
 	tvp = AFSTOV(tvc);
 	if (vnode_get(tvp))
@@ -2788,10 +2700,12 @@ afs_FindVCache(struct VenusFid *afid, afs_int32 * retr
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
@@ -2903,11 +2817,19 @@ afs_NFSFindVCache(struct vcache **avcp, struct VenusFi
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
@@ -2923,21 +2845,6 @@ afs_NFSFindVCache(struct vcache **avcp, struct VenusFi
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
@@ -3054,6 +2961,9 @@ shutdown_vcache(void)
 {
     int i;
     struct afs_cbr *tsp;
+    struct afs_cbr *cbr, *cbr_next;
+    struct server *ts;
+
     /*
      * XXX We may potentially miss some of the vcaches because if when
      * there are no free vcache entries and all the vcache entries are active
@@ -3123,6 +3033,20 @@ shutdown_vcache(void)
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
