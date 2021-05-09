--- src/budb/database.h.orig	2021-01-14 21:08:41 UTC
+++ src/budb/database.h
@@ -311,12 +311,6 @@ extern struct memoryDB db;
 	      (char *)(addr)),					      \
 	     sizeof(afs_int32))
 
-#ifdef notdef
-/* simple min/max macros */
-#define MIN(x,y)        ((x) < (y) ? (x) : (y))
-#define MAX(x,y)        ((x) > (y) ? (x) : (y))
-#endif /* notdef */
-
 struct memoryHashTable *ht_GetType(int type, int *e_sizeP);
 extern afs_uint32 ht_HashEntry(struct memoryHashTable *mht, char *e);
 extern dbadr ht_LookupBucket(struct ubik_trans *ut,
