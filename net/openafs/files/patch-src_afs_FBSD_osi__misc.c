--- src/afs/FBSD/osi_misc.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/FBSD/osi_misc.c
@@ -48,11 +48,7 @@ osi_lookupname(char *aname, enum uio_seg seg, int foll
     }
     *vpp = n.ni_vp;
     /* XXX should we do this?  Usually NOT (matt) */
-#if defined(AFS_FBSD80_ENV)
     /*VOP_UNLOCK(n.ni_vp, 0);*/
-#else
-    VOP_UNLOCK(n.ni_vp, 0, curthread);
-#endif
     NDFREE(&n, NDF_ONLY_PNBUF);
     if (glocked)
 	AFS_GLOCK();
