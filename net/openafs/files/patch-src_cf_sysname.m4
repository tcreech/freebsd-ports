--- src/cf/sysname.m4.orig	2022-12-15 20:10:23 UTC
+++ src/cf/sysname.m4
@@ -34,6 +34,12 @@ else
                         vm=${v#*.}
                         AFS_SYSNAME="amd64_fbsd_${vM}${vm}"
                         ;;
+                aarch64-*-freebsd*.*)
+                        v=${host#*freebsd}
+                        vM=${v%.*}
+                        vm=${v#*.}
+                        AFS_SYSNAME="arm64_fbsd_${vM}${vm}"
+                        ;;
                 i386-*-dragonfly?.*)
                         v=${host#*dragonfly}
                         vM=${v%.*}
