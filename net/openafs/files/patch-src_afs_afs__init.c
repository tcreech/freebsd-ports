--- src/afs/afs_init.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/afs_init.c
@@ -569,7 +569,9 @@ afs_ResourceInit(int preallocs)
     afs_server =
 	rx_NewService(0, RX_STATS_SERVICE_ID, "rpcstats", &secobj, 1,
 		      RXSTATS_ExecuteRequest);
+    AFS_GUNLOCK();
     rx_StartServer(0);
+    AFS_GLOCK();
     afs_osi_Wakeup(&afs_server);	/* wakeup anyone waiting for it */
     return 0;
 
@@ -710,14 +712,23 @@ shutdown_cache(void)
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
@@ -757,7 +768,6 @@ static void
 shutdown_server(void)
 {
     int i;
-    struct afs_cbr *tcbrp, *tbrp;
     struct srvAddr *sa;
 
     for (i = 0; i < NSERVERS; i++) {
@@ -774,13 +784,6 @@ shutdown_server(void)
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
