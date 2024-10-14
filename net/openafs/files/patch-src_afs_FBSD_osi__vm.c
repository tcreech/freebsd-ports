--- src/afs/FBSD/osi_vm.c.orig	2022-12-15 20:10:23 UTC
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
@@ -110,6 +114,7 @@ osi_VM_StoreAllSegments(struct vcache *avc)
 {
     struct vnode *vp;
     struct vm_object *obj;
+    int mightbedirty;
 
     vp = AFSTOV(avc);
     /*
@@ -123,7 +128,13 @@ osi_VM_StoreAllSegments(struct vcache *avc)
 
     obj = vp->v_object;
 
-    if (obj != NULL && (obj->flags & OBJ_MIGHTBEDIRTY) != 0) {
+#if defined(AFS_FBSD_MIGHTBEDIRTY_HELPER)
+    mightbedirty = (obj != NULL && vm_object_mightbedirty(obj));
+#else
+    mightbedirty = (obj != NULL && (obj->flags & OBJ_MIGHTBEDIRTY) != 0);
+#endif
+
+    if (mightbedirty) {
 	ReleaseWriteLock(&avc->lock);
 	AFS_GUNLOCK();
 
@@ -155,7 +166,7 @@ osi_VM_TryToSmush(struct vcache *avc, afs_ucred_t *acr
     vp = AFSTOV(avc);
 
     VI_LOCK(vp);
-    if (vp->v_iflag & VI_DOOMED) {
+    if (VN_IS_DOOMED(vp)) {
 	VI_UNLOCK(vp);
 	return;
     }
