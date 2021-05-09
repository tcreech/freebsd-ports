--- src/WINNT/tests/largefiles/lftest.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/tests/largefiles/lftest.c
@@ -48,12 +48,6 @@ int test_write(HANDLE hf, LARGE_INTEGER offset) {
 	ret = -1;
     }
 
-#if 0
-    if (!FlushFileBuffers(hf)) {
-	fprintf(stderr, "Flush buffers fails at offset 0x%08x:%08x gle = 0x%08x\n",
-		 offset.u.HighPart, offset.u.LowPart, GetLastError());
-    }
-#endif
     return ret;
 }
 
