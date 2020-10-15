--- src/afs/FBSD/osi_misc.c.orig	2020-02-21 20:17:12 UTC
+++ src/afs/FBSD/osi_misc.c
@@ -25,12 +25,8 @@ osi_lookupname(char *aname, enum uio_seg seg, int foll
 	       struct vnode **vpp)
 {
     struct nameidata n;
-    int flags, error, glocked;
+    int flags, error;
 
-    glocked = ISAFS_GLOCK();
-    if (glocked)
-	AFS_GUNLOCK();
-
 #if __FreeBSD_version >= 1000021 /* MPSAFE is gone for good! */
     flags = 0;
 #else
@@ -42,16 +38,12 @@ osi_lookupname(char *aname, enum uio_seg seg, int foll
 	flags |= NOFOLLOW;
     NDINIT(&n, LOOKUP, flags, seg, aname, curthread);
     if ((error = namei(&n)) != 0) {
-	if (glocked)
-	    AFS_GLOCK();
 	return error;
     }
     *vpp = n.ni_vp;
     /* XXX should we do this?  Usually NOT (matt) */
     /*VOP_UNLOCK(n.ni_vp, 0);*/
     NDFREE(&n, NDF_ONLY_PNBUF);
-    if (glocked)
-	AFS_GLOCK();
     return 0;
 }
 
