--- src/vlserver/vlutils.c.orig	2021-01-14 21:08:41 UTC
+++ src/vlserver/vlutils.c
@@ -397,7 +397,32 @@ CheckInit(struct ubik_trans *trans, int builddb)
     return 0;
 }
 
+/**
+ * Grow the eofPtr in the header by 'bump' bytes.
+ *
+ * @param[inout] cheader    VL header
+ * @param[in] bump	    How many bytes to add to eofPtr
+ * @param[out] a_blockindex On success, set to the original eofPtr before we
+ *			    bumped it
+ * @return VL error codes
+ */
+static afs_int32
+grow_eofPtr(struct vlheader *cheader, afs_int32 bump, afs_int32 *a_blockindex)
+{
+    afs_int32 blockindex = ntohl(cheader->vital_header.eofPtr);
 
+    if (blockindex < 0 || blockindex >= MAX_AFS_INT32 - bump) {
+	VLog(0, ("Error: Tried to grow the VLDB beyond the 2GiB limit. Either "
+		 "find a way to trim down your VLDB, or upgrade to a release "
+		 "and database format that supports a larger VLDB.\n"));
+	return VL_IO;
+    }
+
+    *a_blockindex = blockindex;
+    cheader->vital_header.eofPtr = htonl(blockindex + bump);
+    return 0;
+}
+
 afs_int32
 GetExtentBlock(struct vl_ctx *ctx, register afs_int32 base)
 {
@@ -418,16 +443,17 @@ GetExtentBlock(struct vl_ctx *ctx, register afs_int32 
 
 	/* Write the full extension block at end of vldb */
 	ctx->ex_addr[base]->ex_hdrflags = htonl(VLCONTBLOCK);
-	blockindex = ntohl(ctx->cheader->vital_header.eofPtr);
+	code = grow_eofPtr(ctx->cheader, VL_ADDREXTBLK_SIZE, &blockindex);
+	if (code)
+	    ERROR_EXIT(VL_IO);
+
 	code =
 	    vlwrite(ctx->trans, blockindex, (char *)ctx->ex_addr[base],
 		    VL_ADDREXTBLK_SIZE);
 	if (code)
 	    ERROR_EXIT(VL_IO);
 
-	/* Update the cheader.vitalheader structure on disk */
-	ctx->cheader->vital_header.eofPtr = blockindex + VL_ADDREXTBLK_SIZE;
-	ctx->cheader->vital_header.eofPtr = htonl(ctx->cheader->vital_header.eofPtr);
+
 	code = write_vital_vlheader(ctx);
 	if (code)
 	    ERROR_EXIT(VL_IO);
@@ -572,9 +598,11 @@ AllocBlock(struct vl_ctx *ctx, struct nvlentry *tentry
 	    return 0;
 	ctx->cheader->vital_header.freePtr = htonl(tentry->nextIdHash[0]);
     } else {
+	afs_int32 code;
 	/* hosed, nothing on free list, grow file */
-	blockindex = ntohl(ctx->cheader->vital_header.eofPtr);	/* remember this guy */
-	ctx->cheader->vital_header.eofPtr = htonl(blockindex + sizeof(vlentry));
+	code = grow_eofPtr(ctx->cheader, sizeof(vlentry), &blockindex);
+	if (code)
+	    return 0;
     }
     ctx->cheader->vital_header.allocs++;
     if (write_vital_vlheader(ctx))
@@ -866,13 +894,30 @@ UnthreadVLentry(struct vl_ctx *ctx, afs_int32 blockind
 
     /* Take the RO/RW entries of their respective hash linked lists. */
     for (typeindex = ROVOL; typeindex <= BACKVOL; typeindex++) {
-	if ((errorcode = UnhashVolid(ctx, typeindex, blockindex, aentry)))
+	if ((errorcode = UnhashVolid(ctx, typeindex, blockindex, aentry))) {
+	    if (errorcode == VL_NOENT) {
+		VLog(0, ("Unable to unhash vlentry '%s' (address %d) from hash "
+			 "chain for volid %u (type %d).\n",
+			 aentry->name, blockindex, aentry->volumeId[typeindex], typeindex));
+		VLog(0, ("... The VLDB may be partly corrupted; see vldb_check "
+			 "for how to check for and fix errors.\n"));
+		return VL_DBBAD;
+	    }
 	    return errorcode;
+	}
     }
 
     /* Take it out of the Volname hash list */
-    if ((errorcode = UnhashVolname(ctx, blockindex, aentry)))
+    if ((errorcode = UnhashVolname(ctx, blockindex, aentry))) {
+	if (errorcode == VL_NOENT) {
+	    VLog(0, ("Unable to unhash vlentry '%s' (address %d) from name "
+		     "hash chain.\n", aentry->name, blockindex));
+	    VLog(0, ("... The VLDB may be partly corrupted; see vldb_check "
+		     "for how to check for and fix errors.\n"));
+	    return VL_DBBAD;
+	}
 	return errorcode;
+    }
 
     /* Update cheader entry */
     write_vital_vlheader(ctx);
