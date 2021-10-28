--- src/afs/DARWIN/osi_vm.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/DARWIN/osi_vm.c
@@ -166,7 +166,7 @@ osi_VM_Setup(struct vcache *avc, int force)
 #ifndef AFS_DARWIN80_ENV
     if (UBCISVALID(vp) && ((avc->f.states & CStatd) || force)) {
 	if (!UBCINFOEXISTS(vp)) {
-	    osi_vnhold(avc, 0);
+	    osi_Assert(osi_vnhold(avc) == 0);
 	    avc->f.states |= CUBCinit;
 	    AFS_GUNLOCK();
 	    if ((error = ubc_info_init(vp))) {
