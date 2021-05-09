--- src/WINNT/kfw/inc/krb4/conf.h.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/kfw/inc/krb4/conf.h
@@ -47,11 +47,6 @@ typedef void *pointer;
 #endif
 #endif
 
-/* Does your compiler understand "void"? */
-#ifdef notdef
-#define void int
-#endif
-
 /*
  * A few checks to see that necessary definitions are included.
  */
