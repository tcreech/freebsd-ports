--- src/afs/afs_dcache.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_dcache.c
@@ -95,6 +95,8 @@ afs_size_t afs_vmMappingEnd;	/* !< For large files (>=
 int afs_WaitForCacheDrain = 0;
 int afs_TruncateDaemonRunning = 0;
 int afs_CacheTooFull = 0;
+afs_uint32 afs_CacheTooFullCount = 0;
+afs_uint32 afs_WaitForCacheDrainCount = 0;
 
 afs_int32 afs_dcentries;	/*!< In-memory dcache entries */
 
@@ -395,6 +397,7 @@ void
 afs_MaybeWakeupTruncateDaemon(void)
 {
     if (!afs_CacheTooFull && afs_CacheIsTooFull()) {
+	afs_CacheTooFullCount++;
 	afs_CacheTooFull = 1;
 	if (!afs_TruncateDaemonRunning)
 	    afs_osi_Wakeup((int *)afs_CacheTruncateDaemon);
@@ -405,6 +408,21 @@ afs_MaybeWakeupTruncateDaemon(void)
 }
 
 /*!
+ * Wait for cache drain if conditions warrant.
+ *   */
+void
+afs_MaybeWaitForCacheDrain(void)
+{
+    if (afs_blocksUsed - afs_blocksDiscarded >
+	PERCENT(CM_WAITFORDRAINPCT, afs_cacheBlocks)) {
+	if (afs_WaitForCacheDrain == 0)
+	    afs_WaitForCacheDrainCount++;
+	afs_WaitForCacheDrain = 1;
+	afs_osi_Sleep(&afs_WaitForCacheDrain);
+    }
+}
+
+/*!
  * /struct CTD_stats
  *
  * Keep statistics on run time for afs_CacheTruncateDaemon. This is a
@@ -2255,8 +2273,7 @@ afs_GetDCache(struct vcache *avc, afs_size_t abyte,
 	    ReleaseReadLock(&avc->lock);
 	    while ((afs_blocksUsed - afs_blocksDiscarded) >
 		   PERCENT(CM_WAITFORDRAINPCT, afs_cacheBlocks)) {
-		afs_WaitForCacheDrain = 1;
-		afs_osi_Sleep(&afs_WaitForCacheDrain);
+		afs_MaybeWaitForCacheDrain();
 	    }
 	    afs_MaybeFreeDiscardedDCache();
 	    /* need to check if someone else got the chunk first. */
@@ -2880,6 +2897,34 @@ afs_MemGetDSlot(afs_int32 aslot, dslot_state type)
 
 }				/*afs_MemGetDSlot */
 
+static void
+LogCacheError(int aslot, int off, int code, int target_size)
+{
+    struct osi_stat tstat;
+    char *procname;
+
+    if (afs_osi_Stat(afs_cacheInodep, &tstat)) {
+	tstat.size = -1;
+    }
+
+    procname = osi_AllocSmallSpace(AFS_SMALLOCSIZ);
+    if (procname != NULL) {
+	osi_procname(procname, AFS_SMALLOCSIZ);
+	procname[AFS_SMALLOCSIZ-1] = '\0';
+    }
+
+    afs_warn("afs: disk cache read error in CacheItems slot %d "
+	     "off %d/%d code %d/%d pid %d (%s)\n",
+	     aslot, off, (int)tstat.size, code, target_size,
+	     (int)MyPidxx2Pid(MyPidxx),
+	     procname ? procname : "");
+
+    if (procname != NULL) {
+	osi_FreeSmallSpace(procname);
+	procname = NULL;
+    }
+}
+
 unsigned int last_error = 0, lasterrtime = 0;
 
 /*
@@ -2964,15 +3009,8 @@ afs_UFSGetDSlot(afs_int32 aslot, dslot_state type)
 	    /* If we are requesting a non-DSLOT_NEW slot, this is an error.
 	     * non-DSLOT_NEW slots are supposed to already exist, so if we
 	     * failed to read in the slot, something is wrong. */
-	    struct osi_stat tstat;
-	    if (afs_osi_Stat(afs_cacheInodep, &tstat)) {
-		tstat.size = -1;
-	    }
-	    afs_warn("afs: disk cache read error in CacheItems slot %d "
-	             "off %d/%d code %d/%d\n",
-	             (int)aslot,
-	             off, (int)tstat.size,
-	             (int)code, (int)sizeof(struct fcache));
+	    LogCacheError(aslot, off, code, sizeof(struct fcache));
+
 	    /* put tdc back on the free dslot list */
 	    QRemove(&tdc->lruq);
 	    tdc->index = NULLIDX;
@@ -3566,11 +3604,7 @@ afs_ObtainDCacheForWriting(struct vcache *avc, afs_siz
 	    while (afs_blocksUsed >
 		   PERCENT(CM_WAITFORDRAINPCT, afs_cacheBlocks)) {
 		ReleaseWriteLock(&avc->lock);
-		if (afs_blocksUsed - afs_blocksDiscarded >
-		    PERCENT(CM_WAITFORDRAINPCT, afs_cacheBlocks)) {
-		    afs_WaitForCacheDrain = 1;
-		    afs_osi_Sleep(&afs_WaitForCacheDrain);
-		}
+		afs_MaybeWaitForCacheDrain();
 		afs_MaybeFreeDiscardedDCache();
 		afs_MaybeWakeupTruncateDaemon();
 		ObtainWriteLock(&avc->lock, 509);
@@ -3618,9 +3652,9 @@ afs_MakeShadowDir(struct vcache *avc, struct dcache *a
 {
     int i, code, ret_code = 0, written, trans_size;
     struct dcache *new_dc = NULL;
-    struct osi_file *tfile_src, *tfile_dst;
+    struct osi_file *tfile_src = NULL, *tfile_dst = NULL;
     struct VenusFid shadow_fid;
-    char *data;
+    char *data = NULL;
 
     /* Is this a dir? */
     if (vType(avc) != VDIR)
@@ -3675,9 +3709,16 @@ afs_MakeShadowDir(struct vcache *avc, struct dcache *a
 
     /* Open the files. */
     tfile_src = afs_CFileOpen(&adc->f.inode);
+    if (!tfile_src) {
+	ret_code = EIO;
+	goto done;
+    }
+
     tfile_dst = afs_CFileOpen(&new_dc->f.inode);
-    osi_Assert(tfile_src);
-    osi_Assert(tfile_dst);
+    if (!tfile_dst) {
+	ret_code = EIO;
+	goto done;
+    }
 
     /* And now copy dir dcache data into this dcache,
      * 4k at a time.
@@ -3705,10 +3746,14 @@ afs_MakeShadowDir(struct vcache *avc, struct dcache *a
 	written+=trans_size;
     }
 
-    afs_CFileClose(tfile_dst);
-    afs_CFileClose(tfile_src);
+ done:
+    if (tfile_dst)
+	afs_CFileClose(tfile_dst);
+    if (tfile_src)
+	afs_CFileClose(tfile_src);
 
-    afs_osi_Free(data, 4096);
+    if (data)
+	afs_osi_Free(data, 4096);
 
     ReleaseWriteLock(&new_dc->lock);
     afs_PutDCache(new_dc);
@@ -3725,7 +3770,6 @@ afs_MakeShadowDir(struct vcache *avc, struct dcache *a
 	avc->f.shadow.unique = shadow_fid.Fid.Unique;
     }
 
-done:
     return ret_code;
 }
 
