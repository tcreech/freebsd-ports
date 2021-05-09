--- src/tests/mmap-and-read.c.orig	2021-01-14 21:08:41 UTC
+++ src/tests/mmap-and-read.c
@@ -89,11 +89,6 @@ generate_random_file(const char *filename, unsigned np
 
     memcpy(fbuf, buf, sz);
 
-#if 0
-    if (msync(fbuf, sz, MS_SYNC))
-	err(1, "msync");
-#endif
-
     if (munmap(fbuf, sz) != 0)
 	err(1, "munmap");
 
