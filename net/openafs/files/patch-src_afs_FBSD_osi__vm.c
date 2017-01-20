--- src/afs/FBSD/osi_vm.c.orig	2016-12-10 14:08:45 UTC
+++ src/afs/FBSD/osi_vm.c
@@ -141,39 +141,17 @@ osi_VM_StoreAllSegments(struct vcache *a
 {
     struct vnode *vp;
     struct vm_object *obj;
-    int anyio, tries;
 
     ReleaseWriteLock(&avc->lock);
     AFS_GUNLOCK();
-    tries = 5;
     vp = AFSTOV(avc);
 
-    /*
-     * I don't understand this.  Why not just call vm_object_page_clean()
-     * and be done with it?  I particularly don't understand why we're calling
-     * vget() here.  Is there some reason to believe that the vnode might
-     * be being recycled at this point?  I don't think there's any need for
-     * this loop, either -- if we keep the vnode locked all the time,
-     * that and the object lock will prevent any new pages from appearing.
-     * The loop is what causes the race condition.  -GAW
-     */
-    do {
-	anyio = 0;
-	
-	obj = vp->v_object;
-	if (obj != NULL && obj->flags & OBJ_MIGHTBEDIRTY) {
-	    if (!vget(vp, LK_EXCLUSIVE | LK_RETRY, curthread)) {
-		    obj = vp->v_object;
-		    if (obj != NULL) {
-			AFS_VM_OBJECT_WLOCK(obj);
-			vm_object_page_clean(obj, 0, 0, OBJPC_SYNC);
-			AFS_VM_OBJECT_WUNLOCK(obj);
-			anyio = 1;
-		    }
-		    vput(vp);
-		}
-	    }
-    } while (anyio && (--tries > 0));
+    obj = vp->v_object;
+    if (obj != NULL && obj->flags & OBJ_MIGHTBEDIRTY) {
+        AFS_VM_OBJECT_WLOCK(obj);
+        vm_object_page_clean(obj, 0, 0, OBJPC_SYNC);
+        AFS_VM_OBJECT_WUNLOCK(obj);
+    }
     AFS_GLOCK();
     ObtainWriteLock(&avc->lock, 94);
 }
