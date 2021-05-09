--- src/vol/salvaged.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/salvaged.c
@@ -51,13 +51,9 @@
 #endif
 #endif
 #else /* AFS_VFSINCL_ENV */
-#ifdef	AFS_OSF_ENV
-#include <ufs/inode.h>
-#else /* AFS_OSF_ENV */
 #if !defined(AFS_LINUX20_ENV) && !defined(AFS_XBSD_ENV) && !defined(AFS_DARWIN_ENV)
 #include <sys/inode.h>
 #endif
-#endif
 #endif /* AFS_VFSINCL_ENV */
 #endif /* AFS_SGI_ENV */
 #ifdef	AFS_AIX_ENV
@@ -215,11 +211,13 @@ handleit(struct cmd_syndesc *opts, void *arock)
     cmd_OptionAsFlag(opts, OPT_salvagedirs, &RebuildDirs);
     cmd_OptionAsFlag(opts, OPT_blockreads, &forceR);
     if (cmd_OptionAsString(opts, OPT_parallel, &optstring) == 0) {
+	char *input = optstring;
 	if (strncmp(optstring, "all", 3) == 0) {
 	    PartsPerDisk = 1;
+	    input += 3;
 	}
-	if (strlen(optstring) != 0) {
-	    Parallel = atoi(optstring);
+	if (strlen(input) != 0) {
+	    Parallel = atoi(input);
 	    if (Parallel < 1)
 		Parallel = 1;
 	    if (Parallel > MAXPARALLEL) {
