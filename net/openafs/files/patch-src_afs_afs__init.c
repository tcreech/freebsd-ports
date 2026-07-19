--- src/afs/afs_init.c.orig	2024-10-03 22:32:45 UTC
+++ src/afs/afs_init.c
@@ -554,7 +554,9 @@ afs_ResourceInit(int preallocs)
     afs_server =
 	rx_NewService(0, RX_STATS_SERVICE_ID, "rpcstats", &secobj, 1,
 		      RXSTATS_ExecuteRequest);
+    AFS_GUNLOCK();
     rx_StartServer(0);
+    AFS_GLOCK();
     afs_osi_Wakeup(&afs_server);	/* wakeup anyone waiting for it */
     return 0;
 
@@ -695,14 +697,23 @@ shutdown_cache(void)
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
 #endif /* AFS_XBSD_ENV */
 #ifdef AFS_CACHE_VNODE_PATH
@@ -742,7 +753,6 @@ shutdown_server(void)
 shutdown_server(void)
 {
     int i;
-    struct afs_cbr *tcbrp, *tbrp;
     struct srvAddr *sa;
 
     for (i = 0; i < NSERVERS; i++) {
@@ -759,13 +769,6 @@ shutdown_server(void)
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
