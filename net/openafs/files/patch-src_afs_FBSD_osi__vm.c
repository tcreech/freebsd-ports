--- src/afs/FBSD/osi_vm.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/FBSD/osi_vm.c
@@ -50,7 +50,11 @@
 
 #define	lock_vnode(v, f)	vn_lock((v), (f))
 #define ilock_vnode(v)	vn_lock((v), LK_INTERLOCK|LK_EXCLUSIVE|LK_RETRY)
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+#define unlock_vnode(v)	VOP_UNLOCK(v)
+#else
 #define unlock_vnode(v)	VOP_UNLOCK((v), 0)
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
 #define islocked_vnode(v)	VOP_ISLOCKED((v))
 
 #if __FreeBSD_version >= 1000030
@@ -110,41 +114,37 @@ osi_VM_StoreAllSegments(struct vcache *avc)
 {
     struct vnode *vp;
     struct vm_object *obj;
-    int anyio, tries;
+    int mightbedirty;
 
-    ReleaseWriteLock(&avc->lock);
-    AFS_GUNLOCK();
-    tries = 5;
     vp = AFSTOV(avc);
-
     /*
-     * I don't understand this.  Why not just call vm_object_page_clean()
-     * and be done with it?  I particularly don't understand why we're calling
-     * vget() here.  Is there some reason to believe that the vnode might
-     * be being recycled at this point?  I don't think there's any need for
-     * this loop, either -- if we keep the vnode locked all the time,
-     * that and the object lock will prevent any new pages from appearing.
-     * The loop is what causes the race condition.  -GAW
+     * VOP_ISLOCKED may return LK_EXCLOTHER here, since we may be running in a
+     * BOP_STORE background operation, and so we're running in a different
+     * thread than the actual syscall that has the vnode locked. So we cannot
+     * just call ASSERT_VOP_LOCKED (since that will fail if VOP_ISLOCKED
+     * returns LK_EXCLOTHER), and instead we just have our own assert here.
      */
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
-    AFS_GLOCK();
-    ObtainWriteLock(&avc->lock, 94);
+    osi_Assert(VOP_ISLOCKED(vp) != 0);
+
+    obj = vp->v_object;
+
+#if defined(AFS_FBSD_MIGHTBEDIRTY_HELPER)
+    mightbedirty = (obj != NULL && vm_object_mightbedirty(obj));
+#else
+    mightbedirty = (obj != NULL && (obj->flags & OBJ_MIGHTBEDIRTY) != 0);
+#endif
+
+    if (mightbedirty) {
+	ReleaseWriteLock(&avc->lock);
+	AFS_GUNLOCK();
+
+	AFS_VM_OBJECT_WLOCK(obj);
+	vm_object_page_clean(obj, 0, 0, OBJPC_SYNC);
+	AFS_VM_OBJECT_WUNLOCK(obj);
+
+	AFS_GLOCK();
+	ObtainWriteLock(&avc->lock, 94);
+    }
 }
 
 /* Try to invalidate pages, for "fs flush" or "fs flushv"; or
@@ -166,7 +166,7 @@ osi_VM_TryToSmush(struct vcache *avc, afs_ucred_t *acr
     vp = AFSTOV(avc);
 
     VI_LOCK(vp);
-    if (vp->v_iflag & VI_DOOMED) {
+    if (VN_IS_DOOMED(vp)) {
 	VI_UNLOCK(vp);
 	return;
     }
