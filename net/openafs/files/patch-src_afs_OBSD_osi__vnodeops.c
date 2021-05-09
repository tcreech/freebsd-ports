--- src/afs/OBSD/osi_vnodeops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/OBSD/osi_vnodeops.c
@@ -937,14 +937,6 @@ afs_obsd_reclaim(void *v)
     int haveGlock = ISAFS_GLOCK();
     int haveVlock = CheckLock(&afs_xvcache);
 
-#if 0
-    printf("reclaim usecount %d\n", vp->v_usecount);
-    /* OK, there are no internal vrefCounts, so there shouldn't
-     * be any more refs here. */
-    vp->v_data = NULL;		/* remove from vnode */
-    avc->v = NULL;		/* also drop the ptr to vnode */
-    return 0;
-#else
     if (!haveGlock)
 	AFS_GLOCK();
     if (!haveVlock)
@@ -956,7 +948,6 @@ afs_obsd_reclaim(void *v)
     if (!haveGlock)
 	AFS_GUNLOCK();
     return code;
-#endif
 }
 
 #ifdef AFS_OBSD42_ENV
