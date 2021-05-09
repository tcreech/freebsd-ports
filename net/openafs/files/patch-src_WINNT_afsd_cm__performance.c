--- src/WINNT/afsd/cm_performance.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/cm_performance.c
@@ -118,24 +118,6 @@ void cm_PerformanceAddSCache(cm_scache_t *scp)
     }
     lock_ReleaseRead(&scp->rw);
 
-#if 0
-    if (statp->fid.vnode == 1) {
-        cm_volume_t *volp = NULL;
-        cm_cell_t *cellp = NULL;
-        cm_req_t req;
-
-        cm_InitReq(&req);
-
-        cellp = cm_FindCellByID(statp->fid.cell, 0);
-        if (cellp) {
-            if (!cm_FindVolumeByID(cellp, statp->fid.volume, cm_rootUserp, &req, 0, &volp)) {
-                statp->flags |= CM_FIDSTATS_HAVE_VOLUME;
-                cm_PutVolume(volp);
-            }
-        }
-    }
-#endif
-
     cm_PerformanceInsertToHashTable(statp);
 }
 
