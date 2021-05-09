--- src/cf/osconf.m4.orig	2021-01-14 21:08:41 UTC
+++ src/cf/osconf.m4
@@ -672,7 +672,7 @@ if test "x$GCC" = "xyes"; then
     XCFLAGS="${XCFLAGS} -Wall -Wstrict-prototypes -Wold-style-definition -Wpointer-arith"
   fi
   if test "x$enable_checking" != "xno"; then
-    XCFLAGS="${XCFLAGS} -Wall -Wstrict-prototypes -Wold-style-definition -Werror -fdiagnostics-show-option -Wpointer-arith"
+    XCFLAGS="${XCFLAGS} -Wall -Wstrict-prototypes -Wold-style-definition -Werror -fdiagnostics-show-option -Wpointer-arith -fno-common"
     if test "x$enable_checking" != "xall"; then
       CFLAGS_NOERROR="-Wno-error"
       CFLAGS_NOUNUSED="-Wno-unused"
