--- src/afsd/afsd_kernel.c.orig	2021-01-14 21:08:41 UTC
+++ src/afsd/afsd_kernel.c
@@ -494,11 +494,9 @@ afsd_mount_afs(const char *rn, const char *cacheMountD
     if (afsd_verbose)
 	printf("%s: Mounting the AFS root on '%s', flags: %d.\n", rn,
 	    cacheMountDir, mountFlags);
-#if defined(AFS_FBSD60_ENV)
+#if defined(AFS_FBSD_ENV)
     /* data must be non-const non-NULL but is otherwise ignored */
     if ((mount(MOUNT_AFS, cacheMountDir, mountFlags, &mountFlags)) < 0) {
-#elif defined(AFS_FBSD_ENV)
-    if ((mount("AFS", cacheMountDir, mountFlags, (caddr_t) 0)) < 0) {
 #elif defined(AFS_AIX_ENV)
     if (aix_vmount(cacheMountDir)) {
 #elif defined(AFS_HPUX100_ENV)
