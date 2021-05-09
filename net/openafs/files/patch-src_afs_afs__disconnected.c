--- src/afs/afs_disconnected.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_disconnected.c
@@ -709,7 +709,12 @@ afs_ProcessOpCreate(struct vcache *avc, struct vreques
 	}
 	ObtainReadLock(&tdc->lock);
 	tfile = afs_CFileOpen(&tdc->f.inode);
-        osi_Assert(tfile);
+	if (!tfile) {
+	    ReleaseReadLock(&tdc->lock);
+	    afs_PutDCache(tdc);
+	    code = EIO;
+	    goto end;
+	}
 	code = afs_CFileRead(tfile, 0, ttargetName, tlen);
 	ttargetName[tlen-1] = '\0';
 	afs_CFileClose(tfile);
@@ -1159,34 +1164,6 @@ afs_ResyncDisconFiles(struct vrequest *areq, afs_ucred
 	    tvc->f.ddirty_flags &= ~VDisconCreate;
 	    tvc->f.ddirty_flags |= VDisconCreated;
 	}
-#if 0
-  	/* Get server write lock. */
-  	do {
-	    tc = afs_Conn(&tvc->f.fid, areq, SHARED_LOCK, &rxconn);
-  	    if (tc) {
-	    	XSTATS_START_TIME(AFS_STATS_FS_RPCIDX_SETLOCK);
-  		RX_AFS_GUNLOCK();
-		code = RXAFS_SetLock(rxconn,
-					(struct AFSFid *)&tvc->f.fid.Fid,
-					LockWrite,
-					&tsync);
-		RX_AFS_GLOCK();
-		XSTATS_END_TIME;
-	   } else
-		code = -1;
-
-	} while (afs_Analyze(tc,
-			rxconn,
-			code,
-			&tvc->f.fid,
-			areq,
-			AFS_STATS_FS_RPCIDX_SETLOCK,
-			SHARED_LOCK,
-			NULL));
-
-	if (code)
-	    goto next_file;
-#endif
 	if (tvc->f.ddirty_flags & VDisconRename) {
 	    /* If we're renaming the file, do so now */
 	    code = afs_ProcessOpRename(tvc, areq);
@@ -1250,29 +1227,9 @@ afs_ResyncDisconFiles(struct vrequest *areq, afs_ucred
 	}		/* if DV match or client wins policy */
 
 unlock_srv_file:
-	/* Release server write lock. */
-#if 0
-	do {
-	    tc = afs_Conn(&tvc->f.fid, areq, SHARED_LOCK, &rxconn);
-	    if (tc) {
-	    	XSTATS_START_TIME(AFS_STATS_FS_RPCIDX_RELEASELOCK);
-	    	RX_AFS_GUNLOCK();
-		ucode = RXAFS_ReleaseLock(rxconn,
-				(struct AFSFid *) &tvc->f.fid.Fid,
-				&tsync);
-		RX_AFS_GLOCK();
-		XSTATS_END_TIME;
-	    } else
-		ucode = -1;
-	} while (afs_Analyze(tc,
-			rxconn,
-			ucode,
-			&tvc->f.fid,
-			areq,
-			AFS_STATS_FS_RPCIDX_RELEASELOCK,
-			SHARED_LOCK,
-			NULL));
-#endif
+	/* If we ever lock files while replaying changes, we should unlock the
+	 * file here. */
+
 next_file:
 	ObtainWriteLock(&afs_disconDirtyLock, 710);
 	if (code == 0) {
