--- src/tools/dumpscan/intNN.h.orig	2021-01-14 21:08:41 UTC
+++ src/tools/dumpscan/intNN.h
@@ -31,13 +31,6 @@
 
 /* intNN.h - Sized integer types */
 #include <afs/stds.h>
-#if 0
-typedef short afs_int16;
-typedef unsigned short afs_uint16;
-
-typedef long afs_int32;
-typedef unsigned long afs_uint32;
-#endif
 
 
 /* Support for 64-bit integers.
