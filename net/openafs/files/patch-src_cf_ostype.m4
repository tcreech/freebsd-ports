--- src/cf/ostype.m4.orig	2024-10-03 22:32:45 UTC
+++ src/cf/ostype.m4
@@ -64,6 +64,10 @@ case $system in
                 MKAFS_OSTYPE=FBSD
                 AC_MSG_RESULT(amd64_fbsd)
                 ;;
+        aarch64-*-freebsd*)
+                MKAFS_OSTYPE=FBSD
+                AC_MSG_RESULT(arm64_fbsd)
+                ;;
         *-netbsd*)
                 MKAFS_OSTYPE=NBSD
                 AC_MSG_RESULT(nbsd)
