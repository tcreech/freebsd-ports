--- src/util/afs_lhash.c.orig	2021-01-14 21:08:41 UTC
+++ src/util/afs_lhash.c
@@ -455,9 +455,6 @@ afs_lhash_enter(afs_lhash * lh, unsigned key, void *da
      */
     if (lh->ndata > LOAD_FACTOR * lh->ltable) {
 	afs_lhash_expand(lh);
-#if 0
-	printf("lh->p = %d; lh->maxp = %d\n", lh->p, lh->maxp);
-#endif
     }
 #ifdef CHECK_INVARIANTS
     assert(lh->ndata <= LOAD_FACTOR * lh->ltable);
