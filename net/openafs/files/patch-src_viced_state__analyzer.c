--- src/viced/state_analyzer.c.orig	2021-01-14 21:08:41 UTC
+++ src/viced/state_analyzer.c
@@ -1844,33 +1844,6 @@ get_cb_entry(void)
     return 0;
 }
 
-#if 0
-static int
-find_he_by_index(afs_uint32 idx)
-{
-    int i;
-
-    if (get_h_hdr()) {
-	return 1;
-    }
-
-    for (i = 0; i < hdrs.h_hdr.records; i++) {
-	if (get_he(i)) {
-	    fprintf(stderr, "error getting he %d\n", i);
-	    return 1;
-	}
-	if (he_cursor.he.index == idx)
-	    break;
-    }
-
-    if (i < hdrs.h_hdr.records) {
-	dump_this_he();
-	return 0;
-    }
-    return 1;
-}
-#endif
-
 static int
 find_fe_by_index(afs_uint32 idx)
 {
