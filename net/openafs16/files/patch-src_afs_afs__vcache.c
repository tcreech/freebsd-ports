--- src/afs/afs_vcache.c.orig	2016-11-30 20:06:42 UTC
+++ src/afs/afs_vcache.c
@@ -46,6 +46,11 @@
 #include "afs/afs_cbqueue.h"
 #include "afs/afs_osidnlc.h"
 
+#if defined(AFS_FBSD80_ENV)
+#include <vm/vm.h>
+#include <vm/vm_object.h>
+#endif
+
 afs_int32 afs_maxvcount = 0;	/* max number of vcache entries */
 afs_int32 afs_vcount = 0;	/* number of vcache in use now */
 
@@ -1621,7 +1626,6 @@ afs_RemoteLookup(struct VenusFid *afid, 
     return code;
 }
 
-
 /*!
  * afs_GetVCache
  *
@@ -1754,19 +1758,25 @@ afs_GetVCache(struct VenusFid *afid, str
 	if (!iheldthelock)
 	    VOP_UNLOCK(vp, LK_EXCLUSIVE, current_proc());
 #elif defined(AFS_FBSD80_ENV)
-	iheldthelock = VOP_ISLOCKED(vp);
-	if (!iheldthelock) {
-	    /* nosleep/sleep lock order reversal */
-	    int glocked = ISAFS_GLOCK();
-	    if (glocked)
-		AFS_GUNLOCK();
-	    vn_lock(vp, LK_EXCLUSIVE | LK_RETRY);
-	    if (glocked)
-		AFS_GLOCK();
-	}
-	vinvalbuf(vp, V_SAVE, PINOD, 0); /* changed late in 8.0-CURRENT */
-	if (!iheldthelock)
-	    VOP_UNLOCK(vp, 0);
+    // Locking the vp here is very problematic: whether we drop or keep the
+    // GLOCK to do it, deadlocks are possible and do occur.
+    // Nor does it seem feasible to insist that we always have a locked vp at
+    // this point; it may have just been created, or the calling FBSD code may
+    // not have had even known the vp yet. (E.g., when using "afs_lookup".)
+    // For now, only do the vinvalbuf when we already have the vp locked.
+    if (VOP_ISLOCKED(vp) == LK_EXCLUSIVE) {
+        // Skip vinvalbuf if we're a pager or else we deadlock waiting for
+        // ourselves.
+        if( vp && vp->v_bufobj.bo_object &&
+                vp->v_bufobj.bo_object->paging_in_progress) {
+            afs_warn("warning: this object has paging_in_progress=%d!\n",
+                    vp->v_bufobj.bo_object->paging_in_progress);
+            afs_warn("info: object=%p ; vp = %p\n", vp->v_bufobj.bo_object, vp);
+        } else {
+            // Note: we can hold our sx(9)-based GLOCK here.
+            vinvalbuf(vp, V_SAVE, PINOD, 0); /* changed late in 8.0-CURRENT */
+        }
+    }
 #elif defined(AFS_FBSD60_ENV)
 	iheldthelock = VOP_ISLOCKED(vp, curthread);
 	if (!iheldthelock)
