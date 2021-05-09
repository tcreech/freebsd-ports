--- src/WINNT/afsd/cm_vnodeops.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/cm_vnodeops.c
@@ -1913,21 +1913,12 @@ long cm_AssembleLink(cm_scache_t *linkScp, fschar_t *p
         StringCchCopyA(tsp->data,lengthof(tsp->data), linkp);
         code = CM_ERROR_PATH_NOT_COVERED;
     } else if (*linkp == '\\' || *linkp == '/') {
-#if 0
-        /* formerly, this was considered to be from the AFS root,
-         * but this seems to create problems.  instead, we will just
-         * reject the link */
-        StringCchCopyA(tsp->data,lengthof(tsp->data), linkp+1);
-        *newRootScpp = cm_RootSCachep(userp, reqp);
-        cm_HoldSCache(*newRootScpp);
-#else
         /* we still copy the link data into the response so that
          * the user can see what the link points to
          */
         linkScp->fileType = CM_SCACHETYPE_INVALID;
         StringCchCopyA(tsp->data,lengthof(tsp->data), linkp);
         code = CM_ERROR_NOSUCHPATH;
-#endif
     } else {
         /* a relative link */
         StringCchCopyA(tsp->data,lengthof(tsp->data), linkp);
@@ -4739,20 +4730,6 @@ long cm_IntSetLock(cm_scache_t * scp, cm_user_t * user
     afs_uint32 reqflags = reqp->flags;
 
     osi_Log2(afsd_logp, "CALL SetLock scp 0x%p for lock %d", scp, lockType);
-
-#if 0
-    /*
-     * The file server prior to 1.6.2 does not report an accurate value
-     * and callbacks are not issued if the lock is dropped due to expiration.
-     */
-    if ((lockType != LOCKING_ANDX_SHARED_LOCK && scp->fsLockCount != 0) ||
-         (lockType == LOCKING_ANDX_SHARED_LOCK && scp->fsLockCount < 0))
-    {
-        code = CM_ERROR_LOCK_NOT_GRANTED;
-        osi_Log2(afsd_logp, "CALL SetLock FAILURE, fsLockCount %d code 0x%x", scp->fsLockCount, code);
-        return code;
-    }
-#endif
 
     memset(&volSync, 0, sizeof(volSync));
 
