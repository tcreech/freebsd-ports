--- src/afs/FBSD/osi_misc.c.orig	2026-06-25 20:03:19 UTC
+++ src/afs/FBSD/osi_misc.c
@@ -40,7 +40,11 @@ osi_lookupname(char *aname, enum uio_seg seg, int foll
 	flags |= FOLLOW;
     else
 	flags |= NOFOLLOW;
-    NDINIT(&n, LOOKUP, flags, seg, aname, curthread);
+#if __FreeBSD_version >= 1400043 /* Thread argument removed in 7e1d3eef. */
+    NDINIT(&n, LOOKUP, flags, seg, aname);
+#else
+    NDINIT(&n, LOOKUP, flags, seg, aname , curthread);
+#endif
     if ((error = namei(&n)) != 0) {
 	if (glocked)
 	    AFS_GLOCK();
@@ -49,7 +53,18 @@ osi_lookupname(char *aname, enum uio_seg seg, int foll
     *vpp = n.ni_vp;
     /* XXX should we do this?  Usually NOT (matt) */
     /*VOP_UNLOCK(n.ni_vp, 0);*/
+#if __FreeBSD_version >= 1400074 /* NDFREE retired in 269c564b */
+    /*
+     * NDFREE_PNBUF() is the replacement for NDFREE(&n, NDF_ONLY_PNBUF): it
+     * releases the pathname buffer and leaves the vnode reference alone.
+     * That reference belongs to our caller -- afs_syscall_pioctl() unlocks
+     * and AFS_RELE()s the vnode itself -- so releasing it here makes every
+     * pioctl drop a use count it never acquired.
+     */
+    NDFREE_PNBUF(&n);
+#else
     NDFREE(&n, NDF_ONLY_PNBUF);
+#endif
     if (glocked)
 	AFS_GLOCK();
     return 0;
