--- src/afs/VNOPS/afs_vnop_write.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/VNOPS/afs_vnop_write.c
@@ -163,7 +163,11 @@ afs_UFSWriteUIO(struct vcache *avc, afs_dcache_id_t *i
     AFS_GUNLOCK();
     VOP_LOCK(tfile->vnode, LK_EXCLUSIVE);
     code = VOP_WRITE(tfile->vnode, tuiop, 0, afs_osi_credp);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(tfile->vnode);
+#else
     VOP_UNLOCK(tfile->vnode, 0);
+#endif
     AFS_GLOCK();
 #elif defined(AFS_NBSD_ENV)
     AFS_GUNLOCK();
@@ -436,6 +440,7 @@ int
 afs_DoPartialWrite(struct vcache *avc, struct vrequest *areq)
 {
     afs_int32 code;
+    int sync;
 
     if (afs_stats_cmperf.cacheCurrDirtyChunks <=
 	afs_stats_cmperf.cacheMaxDirtyChunks
@@ -445,14 +450,22 @@ afs_DoPartialWrite(struct vcache *avc, struct vrequest
     afs_Trace2(afs_iclSetp, CM_TRACE_PARTIALWRITE, ICL_TYPE_POINTER, avc,
 	       ICL_TYPE_OFFSET, ICL_HANDLE_OFFSET(avc->f.m.Length));
 
+    sync = AFS_ASYNC;
 #if	defined(AFS_SUN5_ENV)
-    code = afs_StoreAllSegments(avc, areq, AFS_ASYNC | AFS_VMSYNC_INVAL);
-#else
-    code = afs_StoreAllSegments(avc, areq, AFS_ASYNC);
+    sync |= AFS_VMSYNC_INVAL;
+#elif defined(AFS_FBSD_ENV)
+    sync |= AFS_NOVMSYNC;
 #endif
+    code = afs_StoreAllSegments(avc, areq, sync);
     return code;
 }
 
+#ifdef AFS_FBSD_ENV
+static int bkg_store_disabled = 1;
+#else
+static int bkg_store_disabled = 0;
+#endif
+
 /* handle any closing cleanup stuff */
 int
 #if defined(AFS_SGI65_ENV)
@@ -539,7 +552,7 @@ afs_close(OSI_VC_DECL(avc), afs_int32 aflags, afs_ucre
     }
 #endif
     if (aflags & (FWRITE | FTRUNC)) {
-	if (afs_BBusy() || (AFS_NFSXLATORREQ(acred)) || AFS_IS_DISCONNECTED) {
+	if (bkg_store_disabled || afs_BBusy() || (AFS_NFSXLATORREQ(acred)) || AFS_IS_DISCONNECTED) {
 	    /* do it yourself if daemons are all busy */
 	    ObtainWriteLock(&avc->lock, 124);
 	    code = afs_StoreOnLastReference(avc, treq);
@@ -635,8 +648,12 @@ afs_close(OSI_VC_DECL(avc), afs_int32 aflags, afs_ucre
             afs_int32 opens, is_free, is_gone, is_doomed, iflag;
             struct vnode *vp = AFSTOV(avc);
             VI_LOCK(vp);
-            is_doomed =  vp->v_iflag & VI_DOOMED;
+            is_doomed =  VN_IS_DOOMED(vp);
+#if 1
+            is_free = (vp->v_holdcnt > 0);
+#else
             is_free = vp->v_iflag & VI_FREE;
+#endif
             is_gone = vp->v_iflag & VI_DOINGINACT;
             iflag = vp->v_iflag;
             VI_UNLOCK(vp);
