--- src/afs/afs_osi_vm.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/afs_osi_vm.c
@@ -93,13 +93,9 @@ osi_FlushPages(struct vcache *avc, afs_ucred_t *credp)
 	       ICL_TYPE_INT32, origDV.low, ICL_TYPE_INT32, avc->f.m.Length);
 
     ReleaseWriteLock(&avc->lock);
-#ifndef AFS_FBSD_ENV
     AFS_GUNLOCK();
-#endif
     osi_VM_FlushPages(avc, credp);
-#ifndef AFS_FBSD_ENV
     AFS_GLOCK();
-#endif
     ObtainWriteLock(&avc->lock, 88);
 
     /* do this last, and to original version, since stores may occur
