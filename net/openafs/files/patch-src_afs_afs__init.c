--- src/afs/afs_init.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_init.c
@@ -334,7 +334,9 @@ afs_InitVolumeInfo(char *afile)
     if (code)
 	return code;
     tfile = afs_CFileOpen(&volumeInode);
-    osi_Assert(tfile);
+    if (!tfile) {
+	return EIO;
+    }
     afs_CFileTruncate(tfile, 0);
     afs_CFileClose(tfile);
     return 0;
@@ -432,7 +434,7 @@ afs_InitCacheInfo(char *afile)
 #elif defined(AFS_DARWIN80_ENV)
         afs_cacheVfsp = vnode_mount(filevp);
 	if (afs_cacheVfsp && ((st = *(vfs_statfs(afs_cacheVfsp))),1))
-#elif defined(AFS_FBSD80_ENV)
+#elif defined(AFS_FBSD_ENV)
 	if (!VFS_STATFS(filevp->v_mount, &st))
 #elif defined(AFS_NBSD50_ENV)
 	if (!VFS_STATVFS(filevp->v_vfsp, &st))
@@ -567,7 +569,9 @@ afs_ResourceInit(int preallocs)
     afs_server =
 	rx_NewService(0, RX_STATS_SERVICE_ID, "rpcstats", &secobj, 1,
 		      RXSTATS_ExecuteRequest);
+    AFS_GUNLOCK();
     rx_StartServer(0);
+    AFS_GLOCK();
     afs_osi_Wakeup(&afs_server);	/* wakeup anyone waiting for it */
     return 0;
 
@@ -708,14 +712,23 @@ shutdown_cache(void)
 	pag_epoch = 0;
 	pagCounter = 0;
 #if defined(AFS_XBSD_ENV)
-	/* memcache never sets this, so don't panic on shutdown */
-	if (volumeVnode != NULL) {
-	    vrele(volumeVnode);	/* let it go, finally. */
+	{
+	    struct vnode *volumeVnode_free = NULL;
+	    struct vnode *cacheDev_free = NULL;
+	    volumeVnode_free = volumeVnode;
 	    volumeVnode = NULL;
-	}
-	if (cacheDev.held_vnode) {
-	    vrele(cacheDev.held_vnode);
+	    cacheDev_free = cacheDev.held_vnode;
 	    cacheDev.held_vnode = NULL;
+
+	    AFS_GUNLOCK();
+	    /* memcache never sets this, so don't panic on shutdown */
+	    if (volumeVnode_free != NULL) {
+		vrele(volumeVnode_free);	/* let it go, finally. */
+	    }
+	    if (cacheDev_free != NULL) {
+		vrele(cacheDev_free);
+	    }
+	    AFS_GLOCK();
 	}
 #endif
 #ifdef AFS_CACHE_VNODE_PATH
@@ -755,7 +768,6 @@ static void
 shutdown_server(void)
 {
     int i;
-    struct afs_cbr *tcbrp, *tbrp;
     struct srvAddr *sa;
 
     for (i = 0; i < NSERVERS; i++) {
@@ -772,13 +784,6 @@ shutdown_server(void)
                      * here */
                     afs_ReleaseConns(sa->conns);
 		}
-	    }
-	    for (tcbrp = ts->cbrs; tcbrp; tcbrp = tbrp) {
-		/*
-		 * Free all server's callback structs
-		 */
-		tbrp = tcbrp->next;
-		afs_FreeCBR(tcbrp);
 	    }
 	    afs_osi_Free(ts, sizeof(struct server));
 	    ts = next;
