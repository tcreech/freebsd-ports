--- src/afs/DARWIN/osi_vnodeops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/DARWIN/osi_vnodeops.c
@@ -596,12 +596,6 @@ afs_vop_access(ap)
           bits |= PRSFS_INSERT;
        if (ap->a_action & KAUTH_VNODE_DELETE_CHILD)
           bits |= PRSFS_DELETE;
-#if 0 /* I'd argue this should be enforced on the parent. But that's ugly */
-       if (ap->a_action & KAUTH_VNODE_READ_ATTRIBUTES)
-          bits |= PRSFS_LOOKUP;
-       if (ap->a_action & KAUTH_VNODE_READ_SECURITY) /* mode bits/gid, not afs acl */
-          bits |= PRSFS_LOOKUP;
-#endif
     } else {
        if (ap->a_action & KAUTH_VNODE_READ_DATA)
           bits |= PRSFS_READ;
@@ -622,12 +616,6 @@ afs_vop_access(ap)
     }
     if (ap->a_action & KAUTH_VNODE_WRITE_ATTRIBUTES)
        bits |= PRSFS_WRITE;
-#if 0 /* no extended attributes */
-    if (ap->a_action & KAUTH_VNODE_READ_EXTATTRIBUTES)
-       bits |= PRSFS_READ;
-    if (ap->a_action & KAUTH_VNODE_WRITE_EXTATTRIBUTES)
-       bits |= PRSFS_WRITE;
-#endif
     if (ap->a_action & KAUTH_VNODE_WRITE_SECURITY)
        bits |= PRSFS_WRITE;
     /* we can't check for KAUTH_VNODE_TAKE_OWNERSHIP, so we always permit it */
@@ -2046,10 +2034,6 @@ afs_vop_bmap(ap)
     }
     if (ap->a_runp != NULL)
 	*ap->a_runp = 0;
-#ifdef notyet
-    if (ap->a_runb != NULL)
-	*ap->a_runb = 0;
-#endif
 
     return 0;
 }
@@ -2177,12 +2161,6 @@ afs_darwin_finalizevnode(struct vcache *avc, struct vn
 
     if (!(avc->f.states & CDeadVnode) && vnode_vtype(ovp) != VNON) {
 	AFS_GUNLOCK();
-#if 0 /* unsupported */
-        if (dvp && cnp)
-	    vnode_update_identity(ovp, dvp, cnp->cn_nameptr, cnp->cn_namelen,
-				  cnp->cn_hash,
-				  VNODE_UPDATE_PARENT|VNODE_UPDATE_NAME);
-#endif
 	/* Can end up in reclaim... drop GLOCK */
 	vnode_rele(ovp);
 	AFS_GLOCK();
