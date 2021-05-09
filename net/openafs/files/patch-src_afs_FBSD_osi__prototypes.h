--- src/afs/FBSD/osi_prototypes.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/FBSD/osi_prototypes.h
@@ -19,16 +19,6 @@ extern int osi_lookupname(char *aname, enum uio_seg se
 			  struct vnode **vpp);
 extern void *osi_fbsd_alloc(size_t size, int dropglobal);
 extern void osi_fbsd_free(void *p);
-
-/* osi_vfsops.c */
-int afs_init(struct vfsconf *vfc);
-int afs_uninit(struct vfsconf *vfc);
-#if defined(AFS_FBSD80_ENV)
-extern int afs_statfs(struct mount *mp, struct statfs *abp);
-#else
-extern int afs_statfs(struct mount *mp, struct statfs *abp, struct thread *td);
-#endif
-
 extern int osi_fbsd_checkinuse(struct vcache *avc);
 
 #endif /* _OSI_PROTO_H_ */
