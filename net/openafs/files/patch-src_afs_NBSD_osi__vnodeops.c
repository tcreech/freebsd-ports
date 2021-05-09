--- src/afs/NBSD/osi_vnodeops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/NBSD/osi_vnodeops.c
@@ -194,9 +194,6 @@ const struct vnodeopv_entry_desc afs_vnodeop_entries[]
     {&vop_islocked_desc, afs_nbsd_islocked},	/* islocked */
     {&vop_pathconf_desc, afs_nbsd_pathconf},	/* pathconf */
     {&vop_advlock_desc, afs_nbsd_advlock},	/* advlock */
-#if 0
-    {&vop_reallocblks_desc, afs_nbsd_reallocblks},	/* reallocblks */
-#endif
     {&vop_bwrite_desc, vn_bwrite},		/* bwrite */
     {&vop_getpages_desc, genfs_getpages},	/* getpages */
     {&vop_putpages_desc, genfs_putpages},	/* putpages */
@@ -377,13 +374,6 @@ afs_nbsd_lookup(void *v)
 #endif
 
  out:
-#if 0
-#ifdef AFS_NBSD50_ENV
-    if ((afs_debug & AFSDEB_VNLAYER) != 0 && (dvp->v_vflag & VV_ROOT) != 0)
-#else
-    if ((afs_debug & AFSDEB_VNLAYER) != 0 && (dvp->v_flag & VROOT) != 0)
-#endif
-#endif
     if ((afs_debug & AFSDEB_VNLAYER) != 0) {
 	printf("nbsd_lookup done dvp %p cnt %d\n", dvp, dvp->v_usecount);
     }
@@ -638,11 +628,6 @@ afs_nbsd_read(void *v)
 	printf("nbsd_read enter %p vp %p\n", ap, ap->a_vp);
     }
 
-#if 0
-    if (ap->a_uio->uio_offset > ap->a_vp->v_size) {
-	return 0; }
-#endif
-
     AFS_GLOCK();
     code = afs_read(VTOAFS(ap->a_vp), ap->a_uio, ap->a_cred, 0);
     AFS_GUNLOCK();
@@ -1360,15 +1345,6 @@ afs_nbsd_bmap(void *v)
         *ap->a_vpp = ap->a_vp;
     if (ap->a_runp != NULL)
         *ap->a_runp = 1024 * 1024; /* XXX */
-#ifdef notyet
-    if (ap->a_runb != NULL)
-        *ap->a_runb = 0;
-#endif
-
-#if 0
-    if (afs_debug & AFSDEB_VNLAYER)
-        printf("nbsd_bmap: exit %p\n", ap);
-#endif
 
     return (0);
 }
