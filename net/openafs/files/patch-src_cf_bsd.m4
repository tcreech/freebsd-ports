--- src/cf/bsd.m4.orig	2021-01-14 21:08:41 UTC
+++ src/cf/bsd.m4
@@ -10,7 +10,8 @@ if test "x$with_bsd_kernel_build" != "x"; then
 else
     case $AFS_SYSNAME in
         *_fbsd_*)
-            BSD_KERNEL_BUILD="${BSD_KERNEL_PATH}/${HOST_CPU}/compile/GENERIC"
+	    AS_IF([test -d "${BSD_KERNEL_PATH}/${HOST_CPU}/compile/GENERIC"],
+	          [BSD_KERNEL_BUILD="${BSD_KERNEL_PATH}/${HOST_CPU}/compile/GENERIC"])
             ;;
         *_nbsd*)
             BSD_KERNEL_BUILD="${BSD_KERNEL_PATH}/arch/${HOST_CPU}/compile/GENERIC"
