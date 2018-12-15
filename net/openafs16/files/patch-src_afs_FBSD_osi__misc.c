--- src/afs/FBSD/osi_misc.c.orig	2017-03-01 19:06:07 UTC
+++ src/afs/FBSD/osi_misc.c
@@ -25,11 +25,7 @@ osi_lookupname(char *aname, enum uio_seg
 	       struct vnode **vpp)
 {
     struct nameidata n;
-    int flags, error, glocked;
-
-    glocked = ISAFS_GLOCK();
-    if (glocked)
-	AFS_GUNLOCK();
+    int flags, error;
 
 #if __FreeBSD_version >= 1000021 /* MPSAFE is gone for good! */
     flags = 0;
@@ -42,8 +38,6 @@ osi_lookupname(char *aname, enum uio_seg
 	flags |= NOFOLLOW;
     NDINIT(&n, LOOKUP, flags, seg, aname, curthread);
     if ((error = namei(&n)) != 0) {
-	if (glocked)
-	    AFS_GLOCK();
 	return error;
     }
     *vpp = n.ni_vp;
@@ -54,8 +48,6 @@ osi_lookupname(char *aname, enum uio_seg
     VOP_UNLOCK(n.ni_vp, 0, curthread);
 #endif
     NDFREE(&n, NDF_ONLY_PNBUF);
-    if (glocked)
-	AFS_GLOCK();
     return 0;
 }
 
