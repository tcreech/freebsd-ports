--- src/WINNT/afsd/cm_buf.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/cm_buf.c
@@ -2603,37 +2603,6 @@ long buf_ClearRDRFlag(cm_scache_t *scp, char *reason)
     return 0;
 }
 
-#if 0
-long buf_CleanDirtyBuffers(cm_scache_t *scp)
-{
-    cm_buf_t *bp;
-    afs_uint32 bcount = 0;
-    cm_fid_t * fidp = &scp->fid;
-
-    for (bp = cm_data.buf_allp; bp; bp=bp->allp, bcount++) {
-	if (!cm_FidCmp(fidp, &bp->fid) && (bp->flags & CM_BUF_DIRTY)) {
-            buf_Hold(bp);
-	    lock_ObtainMutex(&bp->mx);
-	    _InterlockedAnd(&bp->cmFlags, ~CM_BUF_CMSTORING);
-	    _InterlockedAnd(&bp->flags, ~CM_BUF_DIRTY);
-            bp->dirty_length = 0;
-	    _InterlockedOr(&bp->flags, CM_BUF_ERROR);
-	    bp->error = VNOVNODE;
-	    bp->dataVersion = CM_BUF_VERSION_BAD; /* bad */
-	    bp->dirtyCounter++;
-	    if (bp->flags & CM_BUF_WAITING) {
-		osi_Log2(buf_logp, "BUF CleanDirtyBuffers Waking [scp 0x%x] bp 0x%x", scp, bp);
-		osi_Wakeup((long) &bp);
-	    }
-	    lock_ReleaseMutex(&bp->mx);
-	    buf_Release(bp);
-            buf_DecrementUsedCount();
-	}
-    }
-    return 0;
-}
-#endif
-
 /*
  * The following routines will not be used on a
  * regular basis but are very useful in a variety
