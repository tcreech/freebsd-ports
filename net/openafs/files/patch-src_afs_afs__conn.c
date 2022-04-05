--- src/afs/afs_conn.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/afs_conn.c
@@ -532,7 +532,9 @@ afs_ConnBySA(struct srvAddr *sap, unsigned short aport
 	 */
 	if ((service != 52) && (sap->natping == NULL)) {
 	    sap->natping = tc;
+	    AFS_GUNLOCK();
 	    rx_SetConnSecondsUntilNatPing(tc->id, 20);
+	    AFS_GLOCK();
 	}
 
 	tc->forceConnectFS = 0;	/* apparently we're appropriately connected now */
@@ -542,7 +544,9 @@ afs_ConnBySA(struct srvAddr *sap, unsigned short aport
     } /* end of if (tc->forceConnectFS)*/
 
     *rxconn = tc->id;
+    AFS_GUNLOCK();
     rx_GetConnection(*rxconn);
+    AFS_GLOCK();
 
     ReleaseSharedLock(&afs_xconn);
     return tc;
@@ -672,7 +676,9 @@ afs_PutConn(struct afs_conn *ac, struct rx_connection 
 	          (unsigned long)(uintptrsz)ac, (int)ac->refCount);
     }
     ac->parent->refCount--;
+    AFS_GUNLOCK();
     rx_PutConnection(rxconn);
+    AFS_GLOCK();
 }				/*afs_PutConn */
 
 
