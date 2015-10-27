--- src/afs/FBSD/osi_vnodeops.c.orig	2015-09-16 19:39:46 UTC
+++ src/afs/FBSD/osi_vnodeops.c
@@ -79,7 +79,6 @@ static vop_mkdir_t	afs_vop_mkdir;
 static vop_mknod_t	afs_vop_mknod;
 static vop_open_t	afs_vop_open;
 static vop_pathconf_t	afs_vop_pathconf;
-static vop_poll_t	afs_vop_poll;
 static vop_print_t	afs_vop_print;
 static vop_putpages_t	afs_vop_putpages;
 static vop_read_t	afs_vop_read;
@@ -120,7 +119,6 @@ struct vop_vector afs_vnodeops = {
 	.vop_mknod =		afs_vop_mknod,
 	.vop_open =		afs_vop_open,
 	.vop_pathconf =		afs_vop_pathconf,
-	.vop_poll =		afs_vop_poll,
 	.vop_print =		afs_vop_print,
 	.vop_putpages =		afs_vop_putpages,
 	.vop_read =		afs_vop_read,
@@ -157,7 +155,6 @@ int afs_vop_getpages(struct vop_getpages
 int afs_vop_putpages(struct vop_putpages_args *);
 int afs_vop_ioctl(struct vop_ioctl_args *);
 static int afs_vop_pathconf(struct vop_pathconf_args *);
-int afs_vop_poll(struct vop_poll_args *);
 int afs_vop_fsync(struct vop_fsync_args *);
 int afs_vop_remove(struct vop_remove_args *);
 int afs_vop_link(struct vop_link_args *);
@@ -200,7 +197,7 @@ struct vnodeopv_entry_desc afs_vnodeop_e
     {&vop_mknod_desc, (vop_t *) afs_vop_mknod},	/* mknod */
     {&vop_open_desc, (vop_t *) afs_vop_open},	/* open */
     {&vop_pathconf_desc, (vop_t *) afs_vop_pathconf},	/* pathconf */
-    {&vop_poll_desc, (vop_t *) afs_vop_poll},	/* select */
+    {&vop_poll_desc, (vop_t *) vop_nopoll},	/* select */
     {&vop_print_desc, (vop_t *) afs_vop_print},	/* print */
     {&vop_read_desc, (vop_t *) afs_vop_read},	/* read */
     {&vop_readdir_desc, (vop_t *) afs_vop_readdir},	/* readdir */
@@ -1081,22 +1078,6 @@ afs_vop_ioctl(ap)
     }
 }
 
-/* ARGSUSED */
-int
-afs_vop_poll(ap)
-     struct vop_poll_args	/* {
-				 * struct vnode *a_vp;
-				 * int  a_events;
-				 * struct ucred *a_cred;
-				 * struct thread *td;
-				 * } */ *ap;
-{
-    /*
-     * We should really check to see if I/O is possible.
-     */
-    return (1);
-}
-
 int
 afs_vop_fsync(ap)
      struct vop_fsync_args	/* {
