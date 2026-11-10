--- src/afs/VNOPS/afs_vnop_flock.c.orig	2026-06-25 20:03:19 UTC
+++ src/afs/VNOPS/afs_vnop_flock.c
@@ -631,7 +631,7 @@ int afs_lockctl(struct vcache * avc, struct AFS_FLOCK 
     code = HandleFlock(avc, code, treq, clid, 0 /*!onlymine */ );
     AFS_RWUNLOCK((vnode_t *) avc, VRWLOCK_WRITE);
 #else
-    code = HandleFlock(avc, code, treq, 0, 0 /*!onlymine */ );
+    code = HandleFlock(avc, code, treq, clid, 0 /*!onlymine */ );
 #endif
     code = afs_CheckCode(code, treq, 3);	/* defeat AIX -O bug */
     goto done;
