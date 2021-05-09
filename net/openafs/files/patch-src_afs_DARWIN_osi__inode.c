--- src/afs/DARWIN/osi_inode.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/DARWIN/osi_inode.c
@@ -139,10 +139,6 @@ getinode(fs, dev, inode, vpp, perror)
 #ifdef VFSTOHFS
 		if (!strcmp(mp->mnt_vfc->vfc_name, "hfs")) {
 		    hmp = VFSTOHFS(mp);
-#if 0
-		    if (hmp->hfs_mp == NULL)
-			break;
-#endif
 		    if (hmp->hfs_raw_dev == dev) {
 			fs = hmp->hfs_mp;
 		    }
@@ -217,14 +213,7 @@ iforget(vp)
     if (vp->v_usecount == 1) {
 	vp->v_usecount = 0;
 	VOP_UNLOCK(vp, 0, current_proc());
-#if 0
-	simple_lock(&vnode_free_list_slock);
-	TAILQ_INSERT_TAIL(&vnode_free_list, vp, v_freelist);
-	freevnodes++;
-	simple_unlock(&vnode_free_list_slock);
-#else
 	printf("iforget: leaking vnode\n");
-#endif
     } else {
 	vput(vp);
     }
