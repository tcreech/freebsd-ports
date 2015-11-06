--- src/afs/FBSD/osi_vnodeops.c.orig	2015-10-28 19:22:26 UTC
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
@@ -592,6 +589,7 @@ afs_vop_create(ap)
     GETNAME();
 
     AFS_GLOCK();
+    ap->a_vap->va_mode |= S_IRUSR;
     error =
 	afs_create(VTOAFS(dvp), name, ap->a_vap,
 		   ap->a_vap->va_vaflags & VA_EXCLUSIVE ? EXCL : NONEXCL,
@@ -1081,22 +1079,6 @@ afs_vop_ioctl(ap)
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
@@ -1589,12 +1571,21 @@ afs_vop_advlock(ap)
 				 * int  a_flags;
 				 * } */ *ap;
 {
-    int error;
+    int error, new_a_op;
     struct ucred cr = *osi_curcred();
 
+    new_a_op = ap->a_op;
+    if(ap->a_op == F_UNLCK){
+	/* This makes no sense; I think passing F_UNLCK rather than
+	 * F_SETLCK in FreeBSD's kern_fcntl is a bug. (We certainly aren't
+	 * being asked to F_SETFD, which happens to equal F_UNLCK.) */
+        /* Quietly alter the erroneous op: */
+	new_a_op = F_SETLK;
+    }
+
     AFS_GLOCK();
     error =
-	afs_lockctl(VTOAFS(ap->a_vp), ap->a_fl, ap->a_op, &cr, (int)ap->a_id);
+	afs_lockctl(VTOAFS(ap->a_vp), ap->a_fl, new_a_op, &cr, (int)ap->a_id);
     AFS_GUNLOCK();
     return error;
 }
