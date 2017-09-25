--- src/afs/FBSD/osi_vnodeops.c.orig	2016-12-08 04:01:51 UTC
+++ src/afs/FBSD/osi_vnodeops.c
@@ -336,17 +336,12 @@ afs_vop_unlock(ap)
     int code = 0;
     u_int op;
     op = ((ap->a_flags) | LK_RELEASE) & LK_TYPE_MASK;
-    int glocked = ISAFS_GLOCK();
-    if (glocked)
-	AFS_GUNLOCK();
     if ((op & (op - 1)) != 0) {
       afs_warn("afs_vop_unlock: Shit.\n");
       goto done;
     }
     code = lockmgr(lkp, ap->a_flags | LK_RELEASE, VI_MTX(vp));
  done:
-    if (glocked)
-	AFS_GLOCK();
     return(code);
 #else
     /* possibly in current code path where this
@@ -543,16 +538,21 @@ afs_vop_lookup(ap)
 	ma_vn_lock(vp, LK_EXCLUSIVE | LK_RETRY, p);
 	ma_vn_lock(dvp, LK_EXCLUSIVE | LK_RETRY, p);
 	/* always return the child locked */
+#ifndef AFS_FBSD70_ENV
 	if (lockparent && (flags & ISLASTCN)
 	    && (error = ma_vn_lock(dvp, LK_EXCLUSIVE, p))) {
 	    vput(vp);
 	    DROPNAME();
 	    return (error);
 	}
-    } else if (vp == dvp) {
-	/* they're the same; afs_lookup() already ref'ed the leaf.
-	 * It came in locked, so we don't need to ref OR lock it */
-    } else {
+#endif
+    } else if (vp != dvp) {
+	/* If they were the same, afs_lookup() already ref'ed the leaf.  It
+	 * came in locked, so we didn't need to ref OR lock it.  Otherwise,
+	 * lock dvp and vp according to flags. */
+
+	/* For older FreeBSD, leave the parent locked if
+	 * both LOCKPARENT and ISLASTCN. */
 	if (!lockparent || !(flags & ISLASTCN)) {
 #ifndef AFS_FBSD70_ENV /* 6 too? */
 	    MA_VOP_UNLOCK(dvp, 0, p);	/* done with parent. */
@@ -1570,14 +1601,23 @@ afs_vop_advlock(ap)
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
+	/* Quietly alter the erroneous op: */
+	new_a_op = F_SETLK;
+    }
+
     AFS_GLOCK();
     error =
 	afs_lockctl(VTOAFS(ap->a_vp),
 		ap->a_fl,
-		ap->a_op, &cr,
+		new_a_op, &cr,
 		(int)(intptr_t)ap->a_id);	/* XXX: no longer unique! */
     AFS_GUNLOCK();
     return error;
