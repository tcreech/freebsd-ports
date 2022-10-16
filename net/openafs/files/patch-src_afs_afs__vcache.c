--- src/afs/afs_vcache.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/afs_vcache.c
@@ -1806,6 +1806,7 @@ afs_GetVCache(struct VenusFid *afid, struct vrequest *
      * is locked, and if it isn't, then we gain and drop it around the call
      * to vinvalbuf; otherwise, we leave it alone.
      */
+    ReleaseWriteLock(&tvc->lock);
     {
 	struct vnode *vp = AFSTOV(tvc);
 	int iheldthelock;
@@ -1814,25 +1815,27 @@ afs_GetVCache(struct VenusFid *afid, struct vrequest *
 	iheldthelock = VOP_ISLOCKED(vp);
 	if (!iheldthelock)
 	    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY, current_proc());
-	/* this is messy. we can call fsync which will try to reobtain this */
-	if (VTOAFS(vp) == tvc)
-	  ReleaseWriteLock(&tvc->lock);
 	if (UBCINFOEXISTS(vp)) {
 	  vinvalbuf(vp, V_SAVE, &afs_osi_cred, current_proc(), PINOD, 0);
 	}
-	if (VTOAFS(vp) == tvc)
-	  ObtainWriteLock(&tvc->lock, 954);
 	if (!iheldthelock)
 	    VOP_UNLOCK(vp, LK_EXCLUSIVE, current_proc());
 #elif defined(AFS_FBSD_ENV)
 	AFS_GUNLOCK();
-	iheldthelock = VOP_ISLOCKED(vp);
+	iheldthelock = 0;
+	if (VOP_ISLOCKED(vp) == LK_EXCLUSIVE) {
+	    iheldthelock = 1;
+	}
 	if (!iheldthelock) {
 	    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY);
 	}
 	vinvalbuf(vp, V_SAVE, PINOD, 0); /* changed late in 8.0-CURRENT */
 	if (!iheldthelock)
+#    if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	    VOP_UNLOCK(vp);
+#    else
 	    VOP_UNLOCK(vp, 0);
+#    endif
 	AFS_GLOCK();
 #elif defined(AFS_OBSD_ENV)
 	iheldthelock = VOP_ISLOCKED(vp, curproc);
@@ -1851,6 +1854,11 @@ afs_GetVCache(struct VenusFid *afid, struct vrequest *
 	    VOP_UNLOCK(vp, 0);
 #endif
     }
+    ObtainWriteLock(&tvc->lock, 954);
+    if (tvc->f.states & CStatd) {
+	ReleaseWriteLock(&tvc->lock);
+	return tvc;
+    }
 #endif
 #endif
 
@@ -2953,6 +2961,9 @@ shutdown_vcache(void)
 {
     int i;
     struct afs_cbr *tsp;
+    struct afs_cbr *cbr, *cbr_next;
+    struct server *ts;
+
     /*
      * XXX We may potentially miss some of the vcaches because if when
      * there are no free vcache entries and all the vcache entries are active
@@ -3022,6 +3033,20 @@ shutdown_vcache(void)
 	    afs_vhashT[i] = 0;
 	}
     }
+
+    /*
+     * Remove any reference to CBRs in the server structs before we free the
+     * memory for our CBRs below.
+     */
+    for (i = 0; i < NSERVERS; i++) {
+	for (ts = afs_servers[i]; ts; ts = ts->next) {
+	    for (cbr = ts->cbrs; cbr; cbr = cbr_next) {
+		cbr_next = cbr->next;
+		afs_FreeCBR(cbr);
+	    }
+	}
+    }
+
     /*
      * Free any leftover callback queue
      */
