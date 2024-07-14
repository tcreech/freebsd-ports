--- src/cf/ostype.m4.orig	2022-12-15 20:10:23 UTC
+++ src/cf/ostype.m4
@@ -65,6 +65,10 @@ case $system in
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
