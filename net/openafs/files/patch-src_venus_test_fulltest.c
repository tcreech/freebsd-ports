--- src/venus/test/fulltest.c.orig	2021-01-14 21:08:41 UTC
+++ src/venus/test/fulltest.c
@@ -89,14 +89,6 @@ main(int argc, char **argv)
 	printf("fchmod failed to set mode properly\n");
 	return -1;
     }
-#if 0
-    /* These appear to be defunct routines;
-     * I don't know what, if anything, replaced them */
-    if (osi_ExclusiveLockNoBlock(fd1) < 0)
-	{perror("flock1");return -1;}
-    if (osi_UnLock(fd1) < 0)
-	{perror("flock/unlock");return -1;}
-#endif
 
 /* How about shared lock portability? */
     {
