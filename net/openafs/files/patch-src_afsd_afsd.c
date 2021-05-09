--- src/afsd/afsd.c.orig	2021-01-14 21:08:41 UTC
+++ src/afsd/afsd.c
@@ -496,11 +496,6 @@ afsd_install_events(void)
 		   (NULL, kSCDynamicStoreDomainState,
 		    kSCCompAnyRegex, kSCEntNetIPv4));
 
-#if 0
-	/* This should tell us when the hostname(s) change. do we care? */
-	keys[N] = SCDynamicStoreKeyCreateHostNames (NULL);
-#endif
-
 	if (keys[0] != NULL) {
 	    CFArrayRef pattern_array;
 
@@ -1133,12 +1128,6 @@ doSweepAFSCache(int *vFilesFound,
 	    SetNoBackupAttr(fullpn_CellInfoFile);
 	} else if ((strcmp(currp->d_name, ".") == 0)
 		   || (strcmp(currp->d_name, "..") == 0) ||
-#ifdef AFS_DECOSF_ENV
-		   /* these are magic AdvFS files */
-		   (strcmp(currp->d_name, ".tags") == 0)
-		   || (strcmp(currp->d_name, "quota.user") == 0)
-		   || (strcmp(currp->d_name, "quota.group") == 0) ||
-#endif
 #ifdef AFS_LINUX22_ENV
 		   /* this is the ext3 journal file */
 		   (strcmp(currp->d_name, ".journal") == 0) ||
@@ -2069,20 +2058,6 @@ afsd_run(void)
 	    if (afsd_verbose)
 		printf("%s: cacheFiles autotuned to %d\n", rn, cacheFiles);
 	}
-#if 0
-       /* This actually needs to
-          1) use powers of 2
-          2) not second-guess when a chunksize comes from the command line
-          3) be less, um, small. 2^2??
-       */
-	/* Sanity check chunkSize */
-	i = max(cacheBlocks / 1000, cacheBlocks / cacheFiles);
-	chunkSize = min(chunkSize, i);
-	chunkSize = max(chunkSize, 2);
-	if (afsd_verbose)
-	    printf("%s: chunkSize autotuned to %d\n", rn, chunkSize);
-#endif
-
 	if (!sawDCacheSize) {
 	    dCacheSize = cacheFiles / 2;
 	    if (dCacheSize > 10000) {
