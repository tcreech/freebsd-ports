--- src/volser/vos.c.orig	2021-01-14 21:08:41 UTC
+++ src/volser/vos.c
@@ -58,9 +58,9 @@
 #endif
 
 /* Local Prototypes */
-int PrintDiagnostics(char *astring, afs_int32 acode);
-int GetVolumeInfo(afs_uint32 volid, afs_uint32 *server, afs_int32 *part,
-                  afs_int32 *voltype, struct nvldbentry *rentry);
+static int PrintDiagnostics(char *astring, afs_int32 acode);
+static int GetVolumeInfo(afs_uint32 volid, afs_uint32 *server, afs_int32 *part,
+		  afs_int32 *voltype, struct nvldbentry *rentry);
 
 struct tqElem {
     afs_uint32 volid;
@@ -117,7 +117,6 @@ qInit(struct tqHead *ahead)
     return;
 }
 
-
 static void
 qPut(struct tqHead *ahead, afs_uint32 volid)
 {
@@ -172,7 +171,6 @@ VolNameOK(char *name)
 {
     size_t total;
 
-
     total = strlen(name);
     if (!strcmp(&name[total - 9], ".readonly")) {
 	return 0;
@@ -205,11 +203,10 @@ IsNumeric(char *name)
     return result;
 }
 
-
 /*
  * Parse a server dotted address and return the address in network byte order
  */
-afs_uint32
+static afs_uint32
 GetServerNoresolve(char *aname)
 {
     int b1, b2, b3, b4;
@@ -227,7 +224,7 @@ GetServerNoresolve(char *aname)
 /*
  * Parse a server name/address and return a non-loopback address in network byte order
  */
-afs_uint32
+static afs_uint32
 GetServer(char *aname)
 {
     struct hostent *th;
@@ -283,21 +280,7 @@ GetServer(char *aname)
     return 0;
 }
 
-afs_int32
-GetVolumeType(char *aname)
-{
-
-    if (!strcmp(aname, "ro"))
-	return (ROVOL);
-    else if (!strcmp(aname, "rw"))
-	return (RWVOL);
-    else if (!strcmp(aname, "bk"))
-	return (BACKVOL);
-    else
-	return (-1);
-}
-
-int
+static int
 IsPartValid(afs_int32 partId, afs_uint32 server, afs_int32 *code)
 {
     struct partList dummyPartList;
@@ -317,11 +300,9 @@ IsPartValid(afs_int32 partId, afs_uint32 server, afs_i
     return success;
 }
 
-
-
  /*sends the contents of file associated with <fd> and <blksize>  to Rx Stream
   * associated  with <call> */
-int
+static int
 SendFile(usd_handle_t ufd, struct rx_call *call, long blksize)
 {
     char *buffer = (char *)0;
@@ -348,7 +329,7 @@ SendFile(usd_handle_t ufd, struct rx_call *call, long 
 	error = USD_READ(ufd, buffer, blksize, &nbytes);
 	if (error) {
 	    fprintf(STDERR, "File system read failed: %s\n",
-	            afs_error_message(error));
+		    afs_error_message(error));
 	    break;
 	}
 
@@ -367,7 +348,7 @@ SendFile(usd_handle_t ufd, struct rx_call *call, long 
 
 /* function invoked by UV_RestoreVolume, reads the data from rx_trx_stream and
  * writes it out to the volume. */
-afs_int32
+static afs_int32
 WriteData(struct rx_call *call, void *rock)
 {
     char *filename = (char *) rock;
@@ -393,7 +374,7 @@ WriteData(struct rx_call *call, void *rock)
 	}
 	if (code) {
 	    fprintf(STDERR, "Could not access file '%s': %s\n", filename,
-	            afs_error_message(code));
+		    afs_error_message(code));
 	    error = VOLSERBADOP;
 	    goto wfail;
 	}
@@ -429,7 +410,7 @@ WriteData(struct rx_call *call, void *rock)
 /* Receive data from <call> stream into file associated
  * with <fd> <blksize>
  */
-int
+static int
 ReceiveFile(usd_handle_t ufd, struct rx_call *call, long blksize)
 {
     char *buffer = NULL;
@@ -459,7 +440,7 @@ ReceiveFile(usd_handle_t ufd, struct rx_call *call, lo
 		USD_WRITE(ufd, &buffer[bytesread - bytesleft], bytesleft, &w);
 	    if (error) {
 		fprintf(STDERR, "File system write failed: %s\n",
-		        afs_error_message(error));
+			afs_error_message(error));
 		ERROR_EXIT(-1);
 	    }
 	}
@@ -471,7 +452,7 @@ ReceiveFile(usd_handle_t ufd, struct rx_call *call, lo
     return (error);
 }
 
-afs_int32
+static afs_int32
 DumpFunction(struct rx_call *call, void *rock)
 {
     char *filename = (char *)rock;
@@ -499,7 +480,7 @@ DumpFunction(struct rx_call *call, void *rock)
 	}
 	if (code) {
 	    fprintf(STDERR, "Could not create file '%s': %s\n", filename,
-	            afs_error_message(code));
+		    afs_error_message(code));
 	    ERROR_EXIT(VOLSERBADOP);
 	}
     }
@@ -921,10 +902,10 @@ XDisplayFormat2(volintXInfo *a_xInfoP, afs_uint32 a_se
 	     * Volume's status is OK - all the fields are valid.
 	     */
 
-                static long server_cache = -1, partition_cache = -1;
-                static char hostname[256], address[32], pname[16];
-                int i,ai[] = {VOLINT_STATS_TIME_IDX_0,VOLINT_STATS_TIME_IDX_1,VOLINT_STATS_TIME_IDX_2,
-                              VOLINT_STATS_TIME_IDX_3,VOLINT_STATS_TIME_IDX_4,VOLINT_STATS_TIME_IDX_5};
+		static long server_cache = -1, partition_cache = -1;
+		static char hostname[256], address[32], pname[16];
+		int i,ai[] = {VOLINT_STATS_TIME_IDX_0,VOLINT_STATS_TIME_IDX_1,VOLINT_STATS_TIME_IDX_2,
+			      VOLINT_STATS_TIME_IDX_3,VOLINT_STATS_TIME_IDX_4,VOLINT_STATS_TIME_IDX_5};
 
 		if (a_servID != server_cache) {
 			struct in_addr s;
@@ -943,7 +924,7 @@ XDisplayFormat2(volintXInfo *a_xInfoP, afs_uint32 a_se
 		fprintf(STDOUT, "id\t\t%lu\n", afs_printable_uint32_lu(a_xInfoP->volid));
 		fprintf(STDOUT, "serv\t\t%s\t%s\n", address, hostname);
 		fprintf(STDOUT, "part\t\t%s\n", pname);
-                fprintf(STDOUT, "status\t\tOK\n");
+		fprintf(STDOUT, "status\t\tOK\n");
 		fprintf(STDOUT, "backupID\t%lu\n",
 			afs_printable_uint32_lu(a_xInfoP->backupID));
 		fprintf(STDOUT, "parentID\t%lu\n",
@@ -996,8 +977,6 @@ XDisplayFormat2(volintXInfo *a_xInfoP, afs_uint32 a_se
 		fprintf(STDOUT, "filecount\t%u\n", a_xInfoP->filecount);
 		fprintf(STDOUT, "dayUse\t\t%u\n", a_xInfoP->dayUse);
 
-
-
 		fprintf(STDOUT,"reads_same_net\t%8d\n",a_xInfoP->stat_reads[VOLINT_STATS_SAME_NET]);
 		fprintf(STDOUT,"reads_same_net_auth\t%8d\n",a_xInfoP->stat_reads[VOLINT_STATS_SAME_NET_AUTH]);
 		fprintf(STDOUT,"reads_diff_net\t%8d\n",a_xInfoP->stat_reads[VOLINT_STATS_DIFF_NET]);
@@ -1049,7 +1028,7 @@ XDisplayFormat2(volintXInfo *a_xInfoP, afs_uint32 a_se
 
 	    fprintf(STDOUT, "inUse\t%d\n",a_xInfoP->inUse);
 	    if (a_xInfoP->inUse == 1)
-	    	(*a_totalOKP)++;
+		(*a_totalOKP)++;
 	    else
 		(*a_totalNotOKP)++;
 
@@ -1092,7 +1071,7 @@ DisplayFormat2(long server, long partition, volintInfo
     }
 
     if (pntr->status == VOK)
-        fprintf(STDOUT, "name\t\t%s\n", pntr->name);
+	fprintf(STDOUT, "name\t\t%s\n", pntr->name);
 
     fprintf(STDOUT, "id\t\t%lu\n",
 	    afs_printable_uint32_lu(pntr->volid));
@@ -1572,7 +1551,6 @@ NukeVolume(struct cmd_syndesc *as)
     return code;
 }
 
-
 /*------------------------------------------------------------------------
  * PRIVATE ExamineVolume
  *
@@ -2068,32 +2046,6 @@ CreateVolume(struct cmd_syndesc *as, void *arock)
     return 0;
 }
 
-#if 0
-static afs_int32
-DeleteAll(struct nvldbentry *entry)
-{
-    int i;
-    afs_int32 error, code, curserver, curpart;
-    afs_uint32 volid;
-
-    MapHostToNetwork(entry);
-    error = 0;
-    for (i = 0; i < entry->nServers; i++) {
-	curserver = entry->serverNumber[i];
-	curpart = entry->serverPartition[i];
-	if (entry->serverFlags[i] & VLSF_ROVOL) {
-	    volid = entry->volumeId[ROVOL];
-	} else {
-	    volid = entry->volumeId[RWVOL];
-	}
-	code = UV_DeleteVolume(curserver, curpart, volid);
-	if (code && !error)
-	    error = code;
-    }
-    return error;
-}
-#endif
-
 static int
 DeleteVolume(struct cmd_syndesc *as, void *arock)
 {
@@ -2202,7 +2154,6 @@ DeleteVolume(struct cmd_syndesc *as, void *arock)
 	partition = entry.serverPartition[idx];
     }
 
-
     code = UV_DeleteVolume(server, partition, volid);
     if (code) {
 	PrintDiagnostics("remove", code);
@@ -2492,7 +2443,6 @@ CopyVolume(struct cmd_syndesc *as, void *arock)
     return 0;
 }
 
-
 static int
 ShadowVolume(struct cmd_syndesc *as, void *arock)
 {
@@ -2689,7 +2639,6 @@ ShadowVolume(struct cmd_syndesc *as, void *arock)
     return 0;
 }
 
-
 static int
 CloneVolume(struct cmd_syndesc *as, void *arock)
 {
@@ -2736,7 +2685,7 @@ CloneVolume(struct cmd_syndesc *as, void *arock)
 		    "vos : partition %s does not exist on the server\n",
 		    as->parms[2].items->data);
 	    return ENOENT;
-        }
+	}
     } else {
 	code = GetVolumeInfo(volid, &server, &part, &voltype, &entry);
 	if (code)
@@ -2752,18 +2701,6 @@ CloneVolume(struct cmd_syndesc *as, void *arock)
 		volname, VOLSER_OLDMAXVOLNAME - 1);
 	    return E2BIG;
 	}
-#if 0
-	/*
-	 * In order that you be able to make clones of RO or BK, this
-	 * check must be omitted.
-	 */
-	if (!VolNameOK(volname)) {
-	    fprintf(STDERR,
-		"Illegal volume name %s, should not end in .readonly or .backup\n",
-		volname);
-	    return EINVAL;
-	}
-#endif
 	if (IsNumeric(volname)) {
 	    fprintf(STDERR,
 		"Illegal volume name %s, should not be a number\n",
@@ -2794,7 +2731,6 @@ CloneVolume(struct cmd_syndesc *as, void *arock)
     if (as->parms[6].items) flags |= RV_RDONLY;
     if (as->parms[7].items) flags |= RV_RWONLY;
 
-
     code =
 	UV_CloneVolume(server, part, volid, cloneid, volname, flags);
 
@@ -2809,7 +2745,6 @@ CloneVolume(struct cmd_syndesc *as, void *arock)
     return 0;
 }
 
-
 static int
 BackupVolume(struct cmd_syndesc *as, void *arock)
 {
@@ -2899,7 +2834,7 @@ ReleaseVolume(struct cmd_syndesc *as, void *arock)
 	return EINVAL;
     }
     if (as->parms[3].items) /* -force-reclone */
-        flags |= REL_COMPLETE;
+	flags |= REL_COMPLETE;
 
     avolid = vsu_GetVolumeID(as->parms[0].items->data, cstruct, &err);
     if (avolid == 0) {
@@ -3195,7 +3130,7 @@ RestoreVolumeCmd(struct cmd_syndesc *as, void *arock)
 		   && entry.volumeId[voltype] != avolid) {
 	    avolid = entry.volumeId[voltype];
 	}
-        aparentid = entry.volumeId[RWVOL];
+	aparentid = entry.volumeId[RWVOL];
     }
 
     else {			/* volume exists - do we do a full incremental or abort */
@@ -3210,7 +3145,7 @@ RestoreVolumeCmd(struct cmd_syndesc *as, void *arock)
 		   && entry.volumeId[voltype] != avolid) {
 	    avolid = entry.volumeId[voltype];
 	}
-        aparentid = entry.volumeId[RWVOL];
+	aparentid = entry.volumeId[RWVOL];
 
 	/* A file name was specified  - check if volume is on another partition */
 	vcode = GetVolumeInfo(avolid, &Oserver, &Opart, &Otype, &Oentry);
@@ -3315,20 +3250,15 @@ RestoreVolumeCmd(struct cmd_syndesc *as, void *arock)
 	restoreflags |= RV_NODEL;
     }
 
-
     code =
 	UV_RestoreVolume2(aserver, apart, avolid, aparentid,
-                          avolname, restoreflags, WriteData, afilename);
+			  avolname, restoreflags, WriteData, afilename);
     if (code) {
 	PrintDiagnostics("restore", code);
 	exit(1);
     }
     MapPartIdIntoName(apart, apartName);
 
-    /*
-     * patch typo here - originally "parms[1]", should be "parms[0]"
-     */
-
     fprintf(STDOUT, "Restored volume %s on %s %s\n", avolname,
 	    as->parms[0].items->data, apartName);
     return 0;
@@ -3454,17 +3384,11 @@ RemoveSite(struct cmd_syndesc *as, void *arock)
 		as->parms[1].items->data);
 	exit(1);
     }
-/*
- *skip the partition validity check, since it is possible that the partition
- *has since been decomissioned.
- */
-/*
-	if (!IsPartValid(apart,aserver,&code)){
-	    if(code) PrintError("",code);
-	    else fprintf(STDERR,"vos : partition %s does not exist on the server\n",as->parms[1].items->data);
-	    exit(1);
-	}
-*/
+    /*
+     * The partition validity check is skipped, since it is possible that the
+     * partition has already been decomissioned.
+     */
+
     code = UV_RemoveSite(aserver, apart, avolid);
     if (code) {
 	PrintDiagnostics("remsite", code);
@@ -3542,7 +3466,6 @@ ListPartitions(struct cmd_syndesc *as, void *arock)
 	exit(1);
     }
 
-
     code = UV_ListPartitions(aserver, &dummyPartList, &cnt);
     if (code) {
 	PrintDiagnostics("listpart", code);
@@ -3953,7 +3876,7 @@ SyncServer(struct cmd_syndesc *as, void *arock)
 	}
 	flags = 1;
     } else {
-        pnum = -1;
+	pnum = -1;
     }
 
     if (as->parms[2].items) {
@@ -4259,9 +4182,9 @@ RenameVolume(struct cmd_syndesc *as, void *arock)
     return 0;
 }
 
-int
+static int
 GetVolumeInfo(afs_uint32 volid, afs_uint32 *server, afs_int32 *part, afs_int32 *voltype,
-              struct nvldbentry *rentry)
+	      struct nvldbentry *rentry)
 {
     afs_int32 vcode;
     int i, index = -1;
@@ -4314,8 +4237,8 @@ GetVolumeInfo(afs_uint32 volid, afs_uint32 *server, af
 	return 0;
     }
     fprintf(STDERR,
-            "unexpected volume type for volume %lu\n",
-            (unsigned long)volid);
+	    "unexpected volume type for volume %lu\n",
+	    (unsigned long)volid);
     return -1;
 }
 
@@ -4496,7 +4419,6 @@ DeleteEntry(struct cmd_syndesc *as, void *arock)
     return 0;
 }
 
-
 static int
 CompareVldbEntryByName(const void *p1, const void *p2)
 {
@@ -4507,41 +4429,6 @@ CompareVldbEntryByName(const void *p1, const void *p2)
     return (strcmp(arg1->name, arg2->name));
 }
 
-/*
-static int CompareVldbEntry(char *p1, char *p2)
-{
-    struct nvldbentry *arg1,*arg2;
-    int i;
-    int pos1, pos2;
-    char comp1[100],comp2[100];
-    char temp1[20],temp2[20];
-
-    arg1 = (struct nvldbentry *)p1;
-    arg2 = (struct nvldbentry *)p2;
-    pos1 = -1;
-    pos2 = -1;
-
-    for(i = 0; i < arg1->nServers; i++)
-	if(arg1->serverFlags[i] & VLSF_RWVOL) pos1 = i;
-    for(i = 0; i < arg2->nServers; i++)
-	if(arg2->serverFlags[i] & VLSF_RWVOL) pos2 = i;
-    if(pos1 == -1 || pos2 == -1){
-	pos1 = 0;
-	pos2 = 0;
-    }
-    sprintf(comp1,"%10u",arg1->serverNumber[pos1]);
-    sprintf(comp2,"%10u",arg2->serverNumber[pos2]);
-    sprintf(temp1,"%10u",arg1->serverPartition[pos1]);
-    sprintf(temp2,"%10u",arg2->serverPartition[pos2]);
-    strcat(comp1,temp1);
-    strcat(comp2,temp2);
-    strcat(comp1,arg1->name);
-    strcat(comp1,arg2->name);
-    return(strcmp(comp1,comp2));
-
-}
-
-*/
 static int
 ListVLDB(struct cmd_syndesc *as, void *arock)
 {
@@ -4663,14 +4550,14 @@ ListVLDB(struct cmd_syndesc *as, void *arock)
 	else if (centries > 0) {
 	    if (!tarray) {
 		/* malloc the first bulk entries array */
-                tarraysize = centries * sizeof(struct nvldbentry);
-                tarray = malloc(tarraysize);
+		tarraysize = centries * sizeof(struct nvldbentry);
+		tarray = malloc(tarraysize);
 		if (!tarray) {
 		    fprintf(STDERR,
 			    "Could not allocate enough space for the VLDB entries\n");
 		    goto bypass;
 		}
-                memcpy((char*)tarray, arrayEntries.nbulkentries_val, tarraysize);
+		memcpy((char*)tarray, arrayEntries.nbulkentries_val, tarraysize);
 	    } else {
 		/* Grow the tarray to keep the extra entries */
 		parraysize = (centries * sizeof(struct nvldbentry));
@@ -4690,7 +4577,7 @@ ListVLDB(struct cmd_syndesc *as, void *arock)
 	}
 
 	/* Free the bulk array */
-        xdr_free((xdrproc_t) xdr_nbulkentries, &arrayEntries);
+	xdr_free((xdrproc_t) xdr_nbulkentries, &arrayEntries);
     }
 
     /* Here is where we now sort all the entries and print them */
@@ -5157,19 +5044,19 @@ PrintInt64Size(afs_uint64 in)
     SplitInt64(in,hi,lo);
 
     if (hi == 0) {
-        units = "KB";
+	units = "KB";
     } else if (!(hi & 0xFFFFFC00)) {
-        units = "MB";
-        lo = (hi << 22) | (lo >> 10);
+	units = "MB";
+	lo = (hi << 22) | (lo >> 10);
     } else if (!(hi & 0xFFF00000)) {
-        units = "GB";
-        lo = (hi << 12) | (lo >> 20);
+	units = "GB";
+	lo = (hi << 12) | (lo >> 20);
     } else if (!(hi & 0xC0000000)) {
-        units = "TB";
-        lo = (hi << 2) | (lo >> 30);
+	units = "TB";
+	lo = (hi << 2) | (lo >> 30);
     } else {
-        units = "PB";
-        lo = (hi >> 8);
+	units = "PB";
+	lo = (hi >> 8);
     }
     sprintf(output,"%u %s", lo, units);
     return output;
@@ -5209,7 +5096,7 @@ PartitionInfo(struct cmd_syndesc *as, void *arock)
 	cnt = 1;
     }
     if (as->parms[2].items) {
-        printSummary = 1;
+	printSummary = 1;
     }
     if (apart != -1) {
 	if (!IsPartValid(apart, aserver, &code)) {	/*check for validity of the partition */
@@ -5242,18 +5129,18 @@ PartitionInfo(struct cmd_syndesc *as, void *arock)
 		    "Free space on partition %s: %" AFS_INT64_FMT " K blocks out of total %" AFS_INT64_FMT "\n",
 		    pname, partition.free, partition.minFree);
 	    sumPartitions++;
-            AddUInt64(sumFree,partition.free,&sumFree);
-            AddUInt64(sumStorage,partition.minFree,&sumStorage);
+	    AddUInt64(sumFree,partition.free,&sumFree);
+	    AddUInt64(sumStorage,partition.minFree,&sumStorage);
 	}
     }
     if (printSummary) {
-        fprintf(STDOUT,
+	fprintf(STDOUT,
 		"Summary: %s free out of ",
 		PrintInt64Size(sumFree));
-        fprintf(STDOUT,
-                "%s on %d partitions\n",
-                PrintInt64Size(sumStorage),
-                sumPartitions);
+	fprintf(STDOUT,
+		"%s on %d partitions\n",
+		PrintInt64Size(sumStorage),
+		sumPartitions);
     }
     return 0;
 }
@@ -5414,15 +5301,15 @@ ListAddrs(struct cmd_syndesc *as, void *arock)
     afs_int32 m_nentries;
 
     if (as->parms[0].items && as->parms[1].items) {
-        fprintf(STDERR, "vos: Can't use the -uuid and -host flags together\n");
-        exit(-1);
+	fprintf(STDERR, "vos: Can't use the -uuid and -host flags together\n");
+	exit(-1);
     }
 
     memset(&m_attrs, 0, sizeof(struct ListAddrByAttributes));
     memset(&askuuid, 0, sizeof(afsUUID));
     if (as->parms[0].items) {
 	/* -uuid */
-        if (afsUUID_from_string(as->parms[0].items->data, &askuuid) < 0) {
+	if (afsUUID_from_string(as->parms[0].items->data, &askuuid) < 0) {
 	    fprintf(STDERR, "vos: invalid UUID '%s'\n",
 		    as->parms[0].items->data);
 	    exit(-1);
@@ -5443,8 +5330,8 @@ ListAddrs(struct cmd_syndesc *as, void *arock)
 	m_attrs.Mask = VLADDR_IPADDR;
 	m_attrs.ipaddr = ntohl(saddr);
     } else {
-        /* by index */
-        m_attrs.Mask = VLADDR_INDEX;
+	/* by index */
+	m_attrs.Mask = VLADDR_INDEX;
     }
 
     if (as->parms[2].items) {
@@ -5464,43 +5351,43 @@ ListAddrs(struct cmd_syndesc *as, void *arock)
     }
 
     for (i = 1, m_nentries = 0; nentries; i++) {
-        m_attrs.index = i;
+	m_attrs.index = i;
 
-        xdr_free((xdrproc_t)xdr_bulkaddrs, &m_addrs); /* reset addr list */
-        vcode =
-            ubik_VL_GetAddrsU(cstruct, UBIK_CALL_NEW, &m_attrs, &m_uuid,
-                              &m_uniq, &m_nentries, &m_addrs);
-        switch (vcode) {
-        case 0: /* success */
-            print_addrs(&m_addrs, &m_uuid, m_nentries, printuuid);
-            nentries--;
-            break;
+	xdr_free((xdrproc_t)xdr_bulkaddrs, &m_addrs); /* reset addr list */
+	vcode =
+	    ubik_VL_GetAddrsU(cstruct, UBIK_CALL_NEW, &m_attrs, &m_uuid,
+			      &m_uniq, &m_nentries, &m_addrs);
+	switch (vcode) {
+	case 0: /* success */
+	    print_addrs(&m_addrs, &m_uuid, m_nentries, printuuid);
+	    nentries--;
+	    break;
 
-        case VL_NOENT:
-            if (m_attrs.Mask == VLADDR_UUID) {
-                fprintf(STDERR, "vos: no entry for UUID '%s' found in VLDB\n",
-                        as->parms[0].items->data);
-                exit(-1);
-            } else if (m_attrs.Mask == VLADDR_IPADDR) {
-                fprintf(STDERR, "vos: no entry for host '%s' [0x%08x] found in VLDB\n",
-                        as->parms[1].items->data, m_attrs.ipaddr);
-                exit(-1);
-            }
-            continue;
+	case VL_NOENT:
+	    if (m_attrs.Mask == VLADDR_UUID) {
+		fprintf(STDERR, "vos: no entry for UUID '%s' found in VLDB\n",
+			as->parms[0].items->data);
+		exit(-1);
+	    } else if (m_attrs.Mask == VLADDR_IPADDR) {
+		fprintf(STDERR, "vos: no entry for host '%s' [0x%08x] found in VLDB\n",
+			as->parms[1].items->data, m_attrs.ipaddr);
+		exit(-1);
+	    }
+	    continue;
 
-        case VL_INDEXERANGE:
-            vcode = 0; /* not an error, just means we're done */
-            goto out;
+	case VL_INDEXERANGE:
+	    vcode = 0; /* not an error, just means we're done */
+	    goto out;
 
-        default: /* any other error */
-            fprintf(STDERR, "vos: could not list the server addresses\n");
-            PrintError("", vcode);
-            goto out;
-        }
+	default: /* any other error */
+	    fprintf(STDERR, "vos: could not list the server addresses\n");
+	    PrintError("", vcode);
+	    goto out;
+	}
 
-        /* if -uuid or -host, only list one response */
-        if (as->parms[1].items || as->parms[0].items)
-            break;
+	/* if -uuid or -host, only list one response */
+	if (as->parms[1].items || as->parms[0].items)
+	    break;
     }
 
 out:
@@ -5508,7 +5395,6 @@ out:
     return vcode;
 }
 
-
 static int
 SetAddrs(struct cmd_syndesc *as, void *arock)
 {
@@ -5521,7 +5407,7 @@ SetAddrs(struct cmd_syndesc *as, void *arock)
     memset(&askuuid, 0, sizeof(afsUUID));
     if (as->parms[0].items) {
 	/* -uuid */
-        if (afsUUID_from_string(as->parms[0].items->data, &askuuid) < 0) {
+	if (afsUUID_from_string(as->parms[0].items->data, &askuuid) < 0) {
 	    fprintf(STDERR, "vos: invalid UUID '%s'\n",
 		    as->parms[0].items->data);
 	    exit(-1);
@@ -5634,7 +5520,6 @@ RemoveAddrs(struct cmd_syndesc *as, void *arock)
     return code;
 }
 
-
 static int
 LockEntry(struct cmd_syndesc *as, void *arock)
 {
@@ -5895,7 +5780,7 @@ EndTrans(struct cmd_syndesc *as, void *arock)
     return 0;
 }
 
-int
+static int
 PrintDiagnostics(char *astring, afs_int32 acode)
 {
     switch (acode) {
@@ -5916,7 +5801,6 @@ PrintDiagnostics(char *astring, afs_int32 acode)
     return 0;
 }
 
-
 #ifdef AFS_NT40_ENV
 static DWORD
 win32_enableCrypt(void)
@@ -5928,11 +5812,11 @@ win32_enableCrypt(void)
 
     /* Look up configuration parameters in Registry */
     code = RegOpenKeyEx(HKEY_LOCAL_MACHINE, AFSREG_CLT_SVC_PARAM_SUBKEY,
-                        0, (IsWow64()?KEY_WOW64_64KEY:0)|KEY_QUERY_VALUE, &parmKey);
+			0, (IsWow64()?KEY_WOW64_64KEY:0)|KEY_QUERY_VALUE, &parmKey);
     if (code != ERROR_SUCCESS) {
-        dummyLen = sizeof(cryptall);
-        RegQueryValueEx(parmKey, "SecurityLevel", NULL, NULL,
-                        (BYTE *) &cryptall, &dummyLen);
+	dummyLen = sizeof(cryptall);
+	RegQueryValueEx(parmKey, "SecurityLevel", NULL, NULL,
+			(BYTE *) &cryptall, &dummyLen);
     }
     RegCloseKey (parmKey);
 
@@ -5966,9 +5850,9 @@ MyBeforeProc(struct cmd_syndesc *as, void *arock)
 
     if (as->parms[COMMONPARM_OFFSET_ENCRYPT].items     /* -encrypt specified */
 #ifdef AFS_NT40_ENV
-        || win32_enableCrypt()
+	|| win32_enableCrypt()
 #endif /* AFS_NT40_ENV */
-         )
+	 )
 	secFlags |= AFSCONF_SECOPTS_ALWAYSENCRYPT;
 
     if (as->parms[COMMONPARM_OFFSET_CONFIG].items)   /* -config flag set */
@@ -5993,14 +5877,6 @@ MyBeforeProc(struct cmd_syndesc *as, void *arock)
     return 0;
 }
 
-int
-osi_audit(void)
-{
-/* this sucks but it works for now.
-*/
-    return 0;
-}
-
 #include "AFS_component_version_number.c"
 
 int
@@ -6037,9 +5913,6 @@ main(int argc, char **argv)
 		"initial quota (KB)");
     cmd_AddParm(ts, "-id", CMD_SINGLE, CMD_OPTIONAL, "volume ID");
     cmd_AddParm(ts, "-roid", CMD_SINGLE, CMD_OPTIONAL, "readonly volume ID");
-#ifdef notdef
-    cmd_AddParm(ts, "-minquota", CMD_SINGLE, CMD_OPTIONAL, "");
-#endif
     COMMONPARMS;
 
     ts = cmd_CreateSyntax("remove", DeleteVolume, NULL, 0, "delete a volume");
