--- src/afs/afs_osi_alloc.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_osi_alloc.c
@@ -52,8 +52,6 @@ afs_osi_Alloc(size_t size)
     AFS_STATS(afs_stats_cmperf.OutStandingMemUsage += size);
 #ifdef AFS_LINUX20_ENV
     return osi_linux_alloc(size, 1);
-#elif defined(AFS_FBSD_ENV)
-    return osi_fbsd_alloc(size, 1);
 #else
     return AFS_KALLOC(size);
 #endif
@@ -70,8 +68,6 @@ afs_osi_Free(void *x, size_t asize)
     AFS_STATS(afs_stats_cmperf.OutStandingMemUsage -= asize);
 #if defined(AFS_LINUX20_ENV)
     osi_linux_free(x);
-#elif defined(AFS_FBSD_ENV)
-    osi_fbsd_free(x);
 #else
     AFS_KFREE(x, asize);
 #endif
