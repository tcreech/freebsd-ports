--- src/afs/VNOPS/afs_vnop_attrs.c.orig	2026-06-25 20:03:19 UTC
+++ src/afs/VNOPS/afs_vnop_attrs.c
@@ -122,7 +122,20 @@ afs_CopyOutAttrs(struct vcache *avc, struct vattr *att
     attrs->va_nodeid &= 0x7fffffff;	/* Saber C hates negative inode #s! */
     attrs->va_nlink = fakedir ? 100 : avc->f.m.LinkCount;
     attrs->va_size = fakedir ? 4096 : avc->f.m.Length;
-#if defined(AFS_FBSD_ENV) || defined(AFS_DFBSD_ENV)
+#if defined(AFS_FBSD_ENV)
+    /*
+     * vnode_pager_setsize() manipulates the pager object and requires the
+     * vnode to be locked exclusively; getattr frequently runs with a shared
+     * lock, or with no lock at all, and asserting that is a panic on a debug
+     * kernel - seen on a directory vnode whose object size had to be updated.
+     * Skip the update unless we happen to hold the lock we would need: the
+     * paths which change the size (write, truncate) do hold it, and pages are
+     * purged wholesale when the file's status changes, so a stale object size
+     * here costs nothing more than a page which is read again.
+     */
+    if (VOP_ISLOCKED(vp) == LK_EXCLUSIVE)
+	vnode_pager_setsize(vp, (u_long) attrs->va_size);
+#elif defined(AFS_DFBSD_ENV)
     vnode_pager_setsize(vp, (u_long) attrs->va_size);
 #endif
     attrs->va_atime.tv_sec = attrs->va_mtime.tv_sec = attrs->va_ctime.tv_sec =
