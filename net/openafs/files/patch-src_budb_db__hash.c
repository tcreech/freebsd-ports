--- src/budb/db_hash.c.orig	2021-01-14 21:08:41 UTC
+++ src/budb/db_hash.c
@@ -844,62 +844,6 @@ ht_MoveEntries(struct ubik_trans *ut, struct memoryHas
 }
 
 
-#ifdef notdef
-static afs_int32
-ht_MoveEntries(struct ubik_trans *ut, struct memoryHashTable *mht)
-{
-    afs_uint32 hash;
-    int bo;
-    struct memoryHTBlock *block;
-    afs_int32 code;
-
-    if (mht->oldLength == 0)
-	return 0;
-
-    LogDebug(3, "ht_MoveEntries:\n");
-    /* we assume here that the hash function will map numbers smaller than the
-     * size of the hash table straight through to hash table indexes.
-     */
-    hash = mht->progress;
-
-    /* get hash table block ? */
-    code = ht_GetTableBlock(ut, mht, hash, 1 /*old */ , &block, &bo);
-    if (code)
-	return code;
-
-    if (block == 0)
-	return BUDB_INTERNALERROR;
-
-    do {
-	mht->progress++;
-	if (block->b.bucket[bo]) {
-	    code = ht_HashInList(ut, mht, ntohl(block->b.bucket[bo]));
-	    if (code) {
-		Log("ht_MoveEntries: ht_HashInList failed\n");
-		return (BUDB_IO);
-	    }
-	    code =
-		set_word_addr(ut, block->a, &block->b, &block->b.bucket[bo],
-			      0);
-	    if (code) {
-		Log("ht_MoveEntries: clear old entry failed\n");
-		return BUDB_IO;
-	    }
-	    break;
-	}
-    } while (++bo < nHTBuckets);
-
-    if (mht->progress >= mht->oldLength)
-	return (ht_FreeTable(ut, mht));
-
-    if (set_word_addr(ut, 0, &db.h, &mht->ht->progress, htonl(mht->progress))) {
-	Log("ht_MoveEntries: progress set failed\n");
-	return BUDB_IO;
-    }
-    return 0;
-}
-#endif /* notdef */
-
 afs_int32
 ht_HashIn(struct ubik_trans *ut,
 	  struct memoryHashTable *mht,
@@ -1016,34 +960,6 @@ ht_HashOutT(struct ubik_trans *ut, struct memoryHashTa
 		       ea, e, (dbadr *) (e + mht->threadOffset));
     if (code)
 	return code;
-#if 0
-    net_ea = htonl(ea);
-    unthread_ea = *(afs_int32 *) ((char *)e + mht->threadOffset);
-    if (block->b.bucket[bo] == net_ea) {
-	if (set_word_addr
-	    (ut, block->a, &block->b, &block->b.bucket[bo], unthread_ea))
-	    return BUDB_IO;
-	goto done;
-    }
-    loop_a = ntohl(block->b.bucket[bo]);
-    while (1) {
-	if (dbread
-	    (ut, loop_a + mht->threadOffset, (char *)&next_loop_a,
-	     sizeof(dbadr)))
-	    return BUDB_IO;
-	if (next_loop_a == 0)
-	    return -1;		/* not found */
-	if (net_ea == next_loop_a) {
-	    if (dbwrite
-		(ut, loop_a + mht->threadOffset, (char *)&unthread_ea,
-		 sizeof(dbadr)))
-		return BUDB_IO;
-	    goto done;
-	}
-	loop_a = ntohl(next_loop_a);
-    }
-  done:
-#endif
     pentries = &mht->ht->entries;
     if (set_word_addr
 	(ut, 0, &db.h, pentries, htonl(ntohl(mht->ht->entries) - 1)))
