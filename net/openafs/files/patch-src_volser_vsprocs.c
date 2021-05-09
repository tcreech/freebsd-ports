--- src/volser/vsprocs.c.orig	2021-01-14 21:08:41 UTC
+++ src/volser/vsprocs.c
@@ -159,11 +159,6 @@ do { \
 
 
 /* Protos for static routines */
-#if 0
-static afs_int32 CheckAndDeleteVolume(struct rx_connection *aconn,
-				      afs_int32 apart, afs_uint32 okvol,
-				      afs_uint32 delvol);
-#endif
 static int GetTrans(struct nvldbentry *vldbEntryPtr, afs_int32 index,
 		    struct rx_connection **connPtr, afs_int32 * transPtr,
 		    afs_uint32 * crtimePtr, afs_uint32 * uptimePtr,
@@ -485,57 +480,6 @@ AFSVolTransCreate_retry(struct rx_connection *z_conn,
     return code;
 }
 
-#if 0
-/* if <okvol> is allright(indicated by beibg able to
- * start a transaction, delete the <delvol> */
-static afs_int32
-CheckAndDeleteVolume(struct rx_connection *aconn, afs_int32 apart,
-		     afs_uint32 okvol, afs_uint32 delvol)
-{
-    afs_int32 error, code, tid, rcode;
-    error = 0;
-    code = 0;
-
-    if (okvol == 0) {
-	code = AFSVolTransCreate_retry(aconn, delvol, apart, ITOffline, &tid);
-	if (!error && code)
-	    error = code;
-	code = AFSVolDeleteVolume(aconn, tid);
-	if (!error && code)
-	    error = code;
-	code = AFSVolEndTrans(aconn, tid, &rcode);
-	if (!code)
-	    code = rcode;
-	if (!error && code)
-	    error = code;
-	return error;
-    } else {
-	code = AFSVolTransCreate_retry(aconn, okvol, apart, ITOffline, &tid);
-	if (!code) {
-	    code = AFSVolEndTrans(aconn, tid, &rcode);
-	    if (!code)
-		code = rcode;
-	    if (!error && code)
-		error = code;
-	    code = AFSVolTransCreate_retry(aconn, delvol, apart, ITOffline, &tid);
-	    if (!error && code)
-		error = code;
-	    code = AFSVolDeleteVolume(aconn, tid);
-	    if (!error && code)
-		error = code;
-	    code = AFSVolEndTrans(aconn, tid, &rcode);
-	    if (!code)
-		code = rcode;
-	    if (!error && code)
-		error = code;
-	} else
-	    error = code;
-	return error;
-    }
-}
-
-#endif
-
 /* called by EmuerateEntry, show vldb entry in a reasonable format */
 void
 SubEnumerateEntry(struct nvldbentry *entry)
@@ -545,26 +489,7 @@ SubEnumerateEntry(struct nvldbentry *entry)
     int isMixed = 0;
     char hoststr[16];
 
-#ifdef notdef
-    fprintf(STDOUT, "	readWriteID %-10u ", entry->volumeId[RWVOL]);
     if (entry->flags & VLF_RWEXISTS)
-	fprintf(STDOUT, " valid \n");
-    else
-	fprintf(STDOUT, " invalid \n");
-    fprintf(STDOUT, "	readOnlyID  %-10u ", entry->volumeId[ROVOL]);
-    if (entry->flags & VLF_ROEXISTS)
-	fprintf(STDOUT, " valid \n");
-    else
-	fprintf(STDOUT, " invalid \n");
-    fprintf(STDOUT, "	backUpID    %-10u ", entry->volumeId[BACKVOL]);
-    if (entry->flags & VLF_BACKEXISTS)
-	fprintf(STDOUT, " valid \n");
-    else
-	fprintf(STDOUT, " invalid \n");
-    if ((entry->cloneId != 0) && (entry->flags & VLF_ROEXISTS))
-	fprintf(STDOUT, "    releaseClone %-10u \n", entry->cloneId);
-#else
-    if (entry->flags & VLF_RWEXISTS)
 	fprintf(STDOUT, "    RWrite: %-10u", entry->volumeId[RWVOL]);
     if (entry->flags & VLF_ROEXISTS)
 	fprintf(STDOUT, "    ROnly: %-10u", entry->volumeId[ROVOL]);
@@ -573,7 +498,6 @@ SubEnumerateEntry(struct nvldbentry *entry)
     if ((entry->cloneId != 0) && (entry->flags & VLF_ROEXISTS))
 	fprintf(STDOUT, "    RClone: %-10lu", (unsigned long)entry->cloneId);
     fprintf(STDOUT, "\n");
-#endif
     fprintf(STDOUT, "    number of sites -> %lu\n",
 	    (unsigned long)entry->nServers);
     for (i = 0; i < entry->nServers; i++) {
@@ -868,13 +792,8 @@ UV_AddVLDBEntry(afs_uint32 aserver, afs_int32 apart, c
     entry.flags = VLF_RWEXISTS;	/* this records that rw volume exists */
     entry.serverFlags[0] = VLSF_RWVOL;	/*this rep site has rw  vol */
     entry.volumeId[RWVOL] = aid;
-#ifdef notdef
-    entry.volumeId[ROVOL] = anewid + 1;	/* rw,ro, bk id are related in the default case */
-    entry.volumeId[BACKVOL] = *anewid + 2;
-#else
     entry.volumeId[ROVOL] = 0;
     entry.volumeId[BACKVOL] = 0;
-#endif
     entry.cloneId = 0;
     /*map into right byte order, before passing to xdr, the stuff has to be in host
      * byte order. Xdr converts it into network order */
@@ -1401,7 +1320,6 @@ UV_ConvertRO(afs_uint32 server, afs_uint32 partition, 
 	if (entry->serverFlags[i] & VLSF_RWVOL) {
 	    rwindex = i;
 	    rwserver = entry->serverNumber[i];
-	/*  rwpartition = entry->serverPartition[i]; */
 	    if (roserver)
 		break;
 	} else if ((entry->serverFlags[i] & VLSF_ROVOL) && !roserver) {
@@ -1416,7 +1334,6 @@ UV_ConvertRO(afs_uint32 server, afs_uint32 partition, 
 	    if (same) {
 		roindex = i;
 		roserver = entry->serverNumber[i];
-	/*	ropartition = entry->serverPartition[i]; */
 		if (rwserver)
 		     break;
 	    }
@@ -1540,11 +1457,6 @@ UV_MoveVolume2(afs_uint32 afromvol, afs_uint32 afromse
     int same;
     char hoststr[16];
 
-#ifdef	ENABLE_BUGFIX_1165
-    volEntries volumeInfo;
-    struct volintInfo *infop = 0;
-#endif
-
     islocked = 0;
     fromconn = (struct rx_connection *)0;
     toconn = (struct rx_connection *)0;
@@ -1773,26 +1685,6 @@ UV_MoveVolume2(afs_uint32 afromvol, afs_uint32 afromse
 	fromDate = 0;
     }
 
-
-#ifdef	ENABLE_BUGFIX_1165
-    /*
-     * Get the internal volume state from the source volume. We'll use such info (i.e. dayUse)
-     * to copy it to the new volume (via AFSSetInfo later on) so that when we move volumes we
-     * don't use this information...
-     */
-    volumeInfo.volEntries_val = (volintInfo *) 0;	/*this hints the stub to allocate space */
-    volumeInfo.volEntries_len = 0;
-    code = AFSVolListOneVolume(fromconn, afrompart, afromvol, &volumeInfo);
-    EGOTO1(mfail, code,
-	   "Failed to get the volint Info of the cloned volume %u\n",
-	   afromvol);
-
-    infop = (volintInfo *) volumeInfo.volEntries_val;
-    infop->maxquota = -1;	/* Else it will replace the default quota */
-    infop->creationDate = -1;	/* Else it will use the source creation date */
-    infop->updateDate = -1;	/* Else it will use the source update date */
-#endif
-
     /* create a volume on the target machine */
     volid = afromvol;
     code = DoVolDelete(toconn, volid, atopart,
@@ -1898,15 +1790,6 @@ UV_MoveVolume2(afs_uint32 afromvol, afs_uint32 afromse
 	  "Failed to set the flags to make new source volume online\n");
     VDONE;
 
-#ifdef	ENABLE_BUGFIX_1165
-    VPRINT1("Setting volume status on destination volume %u ...", volid);
-    code = AFSVolSetInfo(toconn, totid, infop);
-    EGOTO1(mfail, code,
-	   "Failed to set volume status on the destination volume %u\n",
-	   volid);
-    VDONE;
-#endif
-
     /* put new volume online */
     VPRINT1("Ending transaction on destination volume %u ...", afromvol);
     code = AFSVolEndTrans(toconn, totid, &rcode);
@@ -1978,21 +1861,6 @@ UV_MoveVolume2(afs_uint32 afromvol, afs_uint32 afromse
 	}
 	/* or drop through */
     }
-#ifdef notdef
-    /* This is tricky.  File server is very stupid, and if you mark the volume
-     * as VTOutOfService, it may mark the *good* instance (if you're moving
-     * between partitions on the same machine) as out of service.  Since
-     * we're cleaning this code up in DEcorum, we're just going to kludge around
-     * it for now by removing this call. */
-    /* already out of service, just zap it now */
-    code =
-	AFSVolSetFlags(fromconn, fromtid, VTDeleteOnSalvage | VTOutOfService);
-    if (code) {
-	fprintf(STDERR,
-		"Failed to set the flags to make the old source volume offline\n");
-	goto mfail;
-    }
-#endif
     if (atoserver != afromserver) {
 	/* set forwarding pointer for moved volumes */
 	VPRINT1("Setting forwarding pointer for volume %u ...", afromvol);
@@ -2130,10 +1998,6 @@ UV_MoveVolume2(afs_uint32 afromvol, afs_uint32 afromse
     }
     if (volName)
 	free(volName);
-#ifdef	ENABLE_BUGFIX_1165
-    if (infop)
-	free(infop);
-#endif
     if (fromconn)
 	rx_DestroyConnection(fromconn);
     if (toconn)
@@ -2281,10 +2145,6 @@ UV_MoveVolume2(afs_uint32 afromvol, afs_uint32 afromse
   done:			/* routine cleanup */
     if (volName)
 	free(volName);
-#ifdef	ENABLE_BUGFIX_1165
-    if (infop)
-	free(infop);
-#endif
     if (fromconn)
 	rx_DestroyConnection(fromconn);
     if (toconn)
@@ -4677,41 +4537,40 @@ UV_RestoreVolume2(afs_uint32 toserver, afs_int32 topar
             noresolve ? afs_inet_ntoa_r(toserver, hoststr) :
 	    hostutil_GetNameByINet(toserver), partName);
     fflush(STDOUT);
-    code =
-	AFSVolCreateVolume(toconn, topart, tovolreal, volsertype, pparentid, &pvolid,
-			   &totid);
-    if (code) {
-	if (flags & RV_FULLRST) {	/* full restore: delete then create anew */
-	    code = DoVolDelete(toconn, pvolid, topart, "the previous", 0,
-			       &tstatus, NULL);
-	    if (code && code != VNOVOL) {
-		error = code;
-		goto refail;
-	    }
 
-	    code =
-		AFSVolCreateVolume(toconn, topart, tovolreal, volsertype, pparentid,
-				   &pvolid, &totid);
+    /*
+     * Obtain a transaction and get the status of the target volume. Create a new
+     * volume if the target volume does not already exist.
+     */
+    memset(&tstatus, 0, sizeof(tstatus));
+    if ((flags & RV_FULLRST) != 0) {
+	/* Full restore: Delete existing volume then create anew. */
+	code = DoVolDelete(toconn, pvolid, topart, "the previous", 0, &tstatus, NULL);
+	if (code && code != VNOVOL) {
+	    error = code;
+	    goto refail;
+	}
+	code = AFSVolCreateVolume(toconn, topart, tovolreal, volsertype, pparentid,
+			          &pvolid, &totid);
+	EGOTO1(refail, code, "Could not create new volume %u\n", pvolid);
+    } else {
+	/* Incremental restore: Obtain a transaction on the existing volume. */
+	code = AFSVolTransCreate_retry(toconn, pvolid, topart, ITOffline, &totid);
+	if (code == 0) {
+	    code = AFSVolGetStatus(toconn, totid, &tstatus);
+	    EGOTO1(refail, code, "Could not get timestamp from volume %u\n", pvolid);
+	} else if (code == VNOVOL) {
+	    code = AFSVolCreateVolume(toconn, topart, tovolreal, volsertype,
+				      pparentid, &pvolid, &totid);
 	    EGOTO1(refail, code, "Could not create new volume %u\n", pvolid);
 	} else {
-	    code =
-		AFSVolTransCreate_retry(toconn, pvolid, topart, ITOffline, &totid);
-	    EGOTO1(refail, code, "Failed to start transaction on %u\n",
-		   pvolid);
-
-	    code = AFSVolGetStatus(toconn, totid, &tstatus);
-	    EGOTO1(refail, code, "Could not get timestamp from volume %u\n",
-		   pvolid);
-
+	    EGOTO1(refail, code, "Failed to start transaction on %u\n", pvolid);
 	}
-	oldCreateDate = tstatus.creationDate;
-	oldUpdateDate = tstatus.updateDate;
-	oldCloneId = tstatus.cloneID;
-	oldBackupId = tstatus.backupID;
-    } else {
-	oldCreateDate = 0;
-	oldUpdateDate = 0;
     }
+    oldCreateDate = tstatus.creationDate;
+    oldUpdateDate = tstatus.updateDate;
+    oldCloneId = tstatus.cloneID;
+    oldBackupId = tstatus.backupID;
 
     cookie.parent = pparentid;
     cookie.type = voltype;
@@ -5248,7 +5107,8 @@ UV_RemoveSite(afs_uint32 server, afs_int32 part, afs_u
 		      LOCKREL_OPCODE | LOCKREL_AFSID | LOCKREL_TIMESTAMP);
 	    return (vcode);
 	}
-	VDONE;
+	fprintf(STDOUT, " done\n");
+	fflush(STDOUT);
     }
     return 0;
 }
@@ -7069,7 +6929,39 @@ UV_RenameVolume(struct nvldbentry *entry, char oldname
 	goto rvfail;
     }
     islocked = 1;
+
+    /*
+     * Match the flags we just set via SetLock,
+     * so we don't invalidate our compare below.
+     */
+    entry->flags &= ~VLOP_ALLOPERS;
+    entry->flags |= VLOP_ADDSITE;
+
+    /*
+     * Now get the entry again (under lock) and
+     * verify the volume hasn't otherwise changed.
+     */
+    vcode = VLDB_GetEntryByID(entry->volumeId[RWVOL], RWVOL, &storeEntry);
+    if (vcode) {
+	fprintf(STDERR,
+		"Could not obtain the VLDB entry for the volume %u\n",
+		entry->volumeId[RWVOL]);
+	error = vcode;
+	goto rvfail;
+    }
+    /* Convert to net order to match entry, which was passed in net order. */
+    MapHostToNetwork(&storeEntry);
+    if (memcmp(entry, &storeEntry, sizeof(*entry)) != 0) {
+	fprintf(STDERR,
+		"VLDB entry for volume %u has changed; "
+		"please reissue the command.\n",
+		entry->volumeId[RWVOL]);
+	error = VL_BADENTRY;	/* an arbitrary choice, but closest to the truth */
+	goto rvfail;
+    }
+
     strncpy(entry->name, newname, VOLSER_OLDMAXVOLNAME);
+    /* Note that we are reusing storeEntry. */
     MapNetworkToHost(entry, &storeEntry);
     vcode = VLDB_ReplaceEntry(entry->volumeId[RWVOL], RWVOL, &storeEntry, 0);
     if (vcode) {
