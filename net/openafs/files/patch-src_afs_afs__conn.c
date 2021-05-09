--- src/afs/afs_conn.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_conn.c
@@ -152,7 +152,10 @@ release_conns_user_server(struct unixuser *xu, struct 
 			if (sa->natping == tc) {
 			    int cin;
 			    struct afs_conn *tcn;
+			    sa->natping = NULL;
 			    for (tcvn = sa->conns; tcvn; tcvn = tcvn->next) {
+				if (sa->natping != NULL)
+				    break;
 				if (tcvn == tcv)
 				    continue;
 				for(cin = 0; cin < CVEC_LEN; ++cin) {
@@ -529,7 +532,9 @@ afs_ConnBySA(struct srvAddr *sap, unsigned short aport
 	 */
 	if ((service != 52) && (sap->natping == NULL)) {
 	    sap->natping = tc;
+	    AFS_GUNLOCK();
 	    rx_SetConnSecondsUntilNatPing(tc->id, 20);
+	    AFS_GLOCK();
 	}
 
 	tc->forceConnectFS = 0;	/* apparently we're appropriately connected now */
@@ -539,7 +544,9 @@ afs_ConnBySA(struct srvAddr *sap, unsigned short aport
     } /* end of if (tc->forceConnectFS)*/
 
     *rxconn = tc->id;
+    AFS_GUNLOCK();
     rx_GetConnection(*rxconn);
+    AFS_GLOCK();
 
     ReleaseSharedLock(&afs_xconn);
     return tc;
@@ -669,7 +676,9 @@ afs_PutConn(struct afs_conn *ac, struct rx_connection 
 	          (unsigned long)(uintptrsz)ac, (int)ac->refCount);
     }
     ac->parent->refCount--;
+    AFS_GUNLOCK();
     rx_PutConnection(rxconn);
+    AFS_GLOCK();
 }				/*afs_PutConn */
 
 
