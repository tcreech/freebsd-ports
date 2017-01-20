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
 
@@ -1754,7 +1759,7 @@ afs_GetVCache(struct VenusFid *afid, str
 	if (!iheldthelock)
 	    VOP_UNLOCK(vp, LK_EXCLUSIVE, current_proc());
 #elif defined(AFS_FBSD80_ENV)
-	iheldthelock = VOP_ISLOCKED(vp);
+	iheldthelock = VOP_ISLOCKED(vp) == LK_EXCLUSIVE;
 	if (!iheldthelock) {
 	    /* nosleep/sleep lock order reversal */
 	    int glocked = ISAFS_GLOCK();
@@ -1764,7 +1769,21 @@ afs_GetVCache(struct VenusFid *afid, str
 	    if (glocked)
 		AFS_GLOCK();
 	}
-	vinvalbuf(vp, V_SAVE, PINOD, 0); /* changed late in 8.0-CURRENT */
+    int glocked_again = ISAFS_GLOCK();
+    // vinvalbuf can sleep; don't hold the lock.
+    if (glocked_again)
+        AFS_GUNLOCK();
+    // Skip vinvalbuf if we're a pager.
+    if( vp && vp->v_bufobj.bo_object &&
+            vp->v_bufobj.bo_object->paging_in_progress) {
+            afs_warn("warning: this object has paging_in_progress=%d!\n",
+                    vp->v_bufobj.bo_object->paging_in_progress);
+            afs_warn("info: object=%p ; vp = %p\n", vp->v_bufobj.bo_object, vp);
+    } else {
+	    vinvalbuf(vp, V_SAVE, PINOD, 0); /* changed late in 8.0-CURRENT */
+    }
+    if (glocked_again)
+        AFS_GLOCK();
 	if (!iheldthelock)
 	    VOP_UNLOCK(vp, 0);
 #elif defined(AFS_FBSD60_ENV)
