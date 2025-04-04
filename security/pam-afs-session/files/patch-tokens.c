--- tokens.c.orig	2015-11-18 14:39:30 UTC
+++ tokens.c
@@ -25,6 +25,7 @@
 #include <fcntl.h>
 #include <pwd.h>
 #include <sys/wait.h>
+#include <signal.h>
 
 #include <internal.h>
 #include <pam-util/args.h>
