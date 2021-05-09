--- src/afs/DARWIN/osi_vm.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/DARWIN/osi_vm.c
@@ -37,12 +37,6 @@ osi_VM_FlushVCache(struct vcache *avc)
     if (!vp)
 	return 0;
     AFS_GUNLOCK();
-#if 0
-    if (!(UBCINFOMISSING(vp) || UBCINFORECLAIMED(vp))) {
-      size=ubc_getsize(vp);
-      kret=ubc_invalidate(vp,0,size); 
-    }
-#endif
     cache_purge(vp);
     AFS_GLOCK();
 
@@ -161,19 +155,6 @@ osi_VM_Truncate(struct vcache *avc, int alen, afs_ucre
 void
 osi_VM_NukePages(struct vnode *vp, off_t offset, off_t size)
 {
-#if 0
-    void *object;
-    struct vcache *avc = VTOAFS(vp);
-
-    offset = trunc_page(offset);
-    size = round_page(size + 1);
-    while (size) {
-	ubc_page_op(vp, (vm_offset_t) offset,
-		    UPL_POP_SET | UPL_POP_BUSY | UPL_POP_DUMP, 0, 0);
-	size -= PAGE_SIZE;
-	offset += PAGE_SIZE;
-    }
-#endif
 }
 
 int
@@ -185,7 +166,7 @@ osi_VM_Setup(struct vcache *avc, int force)
 #ifndef AFS_DARWIN80_ENV
     if (UBCISVALID(vp) && ((avc->f.states & CStatd) || force)) {
 	if (!UBCINFOEXISTS(vp)) {
-	    osi_vnhold(avc, 0);
+	    osi_Assert(osi_vnhold(avc) == 0);
 	    avc->f.states |= CUBCinit;
 	    AFS_GUNLOCK();
 	    if ((error = ubc_info_init(vp))) {
