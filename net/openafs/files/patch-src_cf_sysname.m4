--- src/cf/sysname.m4.orig	2021-01-14 21:08:41 UTC
+++ src/cf/sysname.m4
@@ -214,15 +214,6 @@ else
                 i386-pc-solaris2.11)
                         AFS_SYSNAME="sunx86_511"
                         ;;
-                alpha*-dec-osf4.0*)
-                        AFS_SYSNAME="alpha_dux40"
-                        ;;
-                alpha*-dec-osf5.0*)
-                        AFS_SYSNAME="alpha_dux50"
-                        ;;
-                alpha*-dec-osf5.1*)
-                        AFS_SYSNAME="alpha_dux51"
-                        ;;
                 mips-sgi-irix6.5)
                         AFS_SYSNAME="sgi_65"
                         enable_pam="no"
