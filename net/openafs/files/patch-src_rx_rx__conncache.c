--- src/rx/rx_conncache.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_conncache.c
@@ -142,6 +142,7 @@ rxi_AddCachedConnection(rx_connParts_p parts, struct r
 	new_entry->hasError = 0;
 	opr_queue_Prepend(&rxi_connectionCache, &new_entry->queue);
     }
+    (*conn)->flags |= RX_CONN_CACHED;
 
     /*
      * if malloc fails, we fail silently
@@ -258,6 +259,19 @@ rx_ReleaseCachedConnection(struct rx_connection *conn)
     struct opr_queue *cursor, *store;
 
     LOCK_CONN_CACHE;
+
+    /* Check if the caller is asking us to release a conn that did NOT come
+     * from the connection cache.  If so, don't bother searching the cache
+     * because the connection won't be found or destroyed.  Since we return
+     * void, the caller must assume the connection _has_ been found and
+     * destroyed. So to avoid leaking the connection, just destroy it now and
+     * return.
+     */
+    if (!(conn->flags & RX_CONN_CACHED)) {
+	rxi_DestroyConnection(conn);
+	UNLOCK_CONN_CACHE;
+	return;
+    }
     for (opr_queue_ScanSafe(&rxi_connectionCache, cursor, store)) {
 	struct cache_entry *cacheConn
 	    = opr_queue_Entry(cursor, struct cache_entry, queue);
@@ -274,6 +288,7 @@ rx_ReleaseCachedConnection(struct rx_connection *conn)
 		cacheConn->hasError = 1;
 		if (cacheConn->inUse == 0) {
 		    opr_queue_Remove(&cacheConn->queue);
+		    cacheConn->conn->flags &= ~RX_CONN_CACHED;
 		    rxi_DestroyConnection(cacheConn->conn);
 		    free(cacheConn);
 		}
