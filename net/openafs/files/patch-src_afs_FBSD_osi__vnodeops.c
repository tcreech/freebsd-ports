--- src/afs/FBSD/osi_vnodeops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/FBSD/osi_vnodeops.c
@@ -61,211 +61,38 @@
 #include <vm/vm_object.h>
 #include <vm/vm_pager.h>
 #include <vm/vnode_pager.h>
-extern int afs_pbuf_freecnt;
+#include <sys/vmmeter.h>
 
-#ifdef AFS_FBSD60_ENV
-static vop_access_t	afs_vop_access;
-static vop_advlock_t	afs_vop_advlock;
-static vop_close_t	afs_vop_close;
-static vop_create_t	afs_vop_create;
-static vop_fsync_t	afs_vop_fsync;
-static vop_getattr_t	afs_vop_getattr;
-static vop_getpages_t	afs_vop_getpages;
-static vop_inactive_t	afs_vop_inactive;
-static vop_ioctl_t	afs_vop_ioctl;
-static vop_link_t	afs_vop_link;
-static vop_lookup_t	afs_vop_lookup;
-static vop_mkdir_t	afs_vop_mkdir;
-static vop_mknod_t	afs_vop_mknod;
-static vop_open_t	afs_vop_open;
-static vop_pathconf_t	afs_vop_pathconf;
-static vop_print_t	afs_vop_print;
-static vop_putpages_t	afs_vop_putpages;
-static vop_read_t	afs_vop_read;
-static vop_readdir_t	afs_vop_readdir;
-static vop_readlink_t	afs_vop_readlink;
-static vop_reclaim_t	afs_vop_reclaim;
-static vop_remove_t	afs_vop_remove;
-static vop_rename_t	afs_vop_rename;
-static vop_rmdir_t	afs_vop_rmdir;
-static vop_setattr_t	afs_vop_setattr;
-static vop_strategy_t	afs_vop_strategy;
-static vop_symlink_t	afs_vop_symlink;
-static vop_write_t	afs_vop_write;
-#if defined(AFS_FBSD70_ENV) && !defined(AFS_FBSD80_ENV)
-static vop_lock1_t      afs_vop_lock;
-static vop_unlock_t     afs_vop_unlock;
-static vop_islocked_t   afs_vop_islocked;
+#if defined(AFS_FBSD_UMA_BUFS)
+extern uma_zone_t afs_pbuf_zone;
+#else
+extern int afs_pbuf_freecnt;
 #endif
 
-struct vop_vector afs_vnodeops = {
-	.vop_default =		&default_vnodeops,
-	.vop_access =		afs_vop_access,
-	.vop_advlock =		afs_vop_advlock,
-	.vop_close =		afs_vop_close,
-	.vop_create =		afs_vop_create,
-	.vop_fsync =		afs_vop_fsync,
-	.vop_getattr =		afs_vop_getattr,
-	.vop_getpages =		afs_vop_getpages,
-	.vop_inactive =		afs_vop_inactive,
-	.vop_ioctl =		afs_vop_ioctl,
-#if !defined(AFS_FBSD80_ENV)
-	/* removed at least temporarily (NFSv4 flux) */
-	.vop_lease =		VOP_NULL,
-#endif
-	.vop_link =		afs_vop_link,
-	.vop_lookup =		afs_vop_lookup,
-	.vop_mkdir =		afs_vop_mkdir,
-	.vop_mknod =		afs_vop_mknod,
-	.vop_open =		afs_vop_open,
-	.vop_pathconf =		afs_vop_pathconf,
-	.vop_print =		afs_vop_print,
-	.vop_putpages =		afs_vop_putpages,
-	.vop_read =		afs_vop_read,
-	.vop_readdir =		afs_vop_readdir,
-	.vop_readlink =		afs_vop_readlink,
-	.vop_reclaim =		afs_vop_reclaim,
-	.vop_remove =		afs_vop_remove,
-	.vop_rename =		afs_vop_rename,
-	.vop_rmdir =		afs_vop_rmdir,
-	.vop_setattr =		afs_vop_setattr,
-	.vop_strategy =		afs_vop_strategy,
-	.vop_symlink =		afs_vop_symlink,
-	.vop_write =		afs_vop_write,
-#if defined(AFS_FBSD70_ENV) && !defined(AFS_FBSD80_ENV)
-	.vop_lock1 =            afs_vop_lock,
-	.vop_unlock =           afs_vop_unlock,
-	.vop_islocked =         afs_vop_islocked,
-#endif
-};
-
-#else /* AFS_FBSD60_ENV */
-
-int afs_vop_lookup(struct vop_lookup_args *);
-int afs_vop_create(struct vop_create_args *);
-int afs_vop_mknod(struct vop_mknod_args *);
-int afs_vop_open(struct vop_open_args *);
-int afs_vop_close(struct vop_close_args *);
-int afs_vop_access(struct vop_access_args *);
-int afs_vop_getattr(struct vop_getattr_args *);
-int afs_vop_setattr(struct vop_setattr_args *);
-int afs_vop_read(struct vop_read_args *);
-int afs_vop_write(struct vop_write_args *);
-int afs_vop_getpages(struct vop_getpages_args *);
-int afs_vop_putpages(struct vop_putpages_args *);
-int afs_vop_ioctl(struct vop_ioctl_args *);
-static int afs_vop_pathconf(struct vop_pathconf_args *);
-int afs_vop_fsync(struct vop_fsync_args *);
-int afs_vop_remove(struct vop_remove_args *);
-int afs_vop_link(struct vop_link_args *);
-int afs_vop_rename(struct vop_rename_args *);
-int afs_vop_mkdir(struct vop_mkdir_args *);
-int afs_vop_rmdir(struct vop_rmdir_args *);
-int afs_vop_symlink(struct vop_symlink_args *);
-int afs_vop_readdir(struct vop_readdir_args *);
-int afs_vop_readlink(struct vop_readlink_args *);
-int afs_vop_inactive(struct vop_inactive_args *);
-int afs_vop_reclaim(struct vop_reclaim_args *);
-int afs_vop_bmap(struct vop_bmap_args *);
-int afs_vop_strategy(struct vop_strategy_args *);
-int afs_vop_print(struct vop_print_args *);
-int afs_vop_advlock(struct vop_advlock_args *);
-
-
-
-/* Global vfs data structures for AFS. */
-vop_t **afs_vnodeop_p;
-struct vnodeopv_entry_desc afs_vnodeop_entries[] = {
-    {&vop_default_desc, (vop_t *) vop_defaultop},
-    {&vop_access_desc, (vop_t *) afs_vop_access},	/* access */
-    {&vop_advlock_desc, (vop_t *) afs_vop_advlock},	/* advlock */
-    {&vop_bmap_desc, (vop_t *) afs_vop_bmap},	/* bmap */
-    {&vop_close_desc, (vop_t *) afs_vop_close},	/* close */
-    {&vop_createvobject_desc, (vop_t *) vop_stdcreatevobject},
-    {&vop_destroyvobject_desc, (vop_t *) vop_stddestroyvobject},
-    {&vop_create_desc, (vop_t *) afs_vop_create},	/* create */
-    {&vop_fsync_desc, (vop_t *) afs_vop_fsync},	/* fsync */
-    {&vop_getattr_desc, (vop_t *) afs_vop_getattr},	/* getattr */
-    {&vop_getpages_desc, (vop_t *) afs_vop_getpages},	/* read */
-    {&vop_getvobject_desc, (vop_t *) vop_stdgetvobject},
-    {&vop_putpages_desc, (vop_t *) afs_vop_putpages},	/* write */
-    {&vop_inactive_desc, (vop_t *) afs_vop_inactive},	/* inactive */
-    {&vop_lease_desc, (vop_t *) vop_null},
-    {&vop_link_desc, (vop_t *) afs_vop_link},	/* link */
-    {&vop_lookup_desc, (vop_t *) afs_vop_lookup},	/* lookup */
-    {&vop_mkdir_desc, (vop_t *) afs_vop_mkdir},	/* mkdir */
-    {&vop_mknod_desc, (vop_t *) afs_vop_mknod},	/* mknod */
-    {&vop_open_desc, (vop_t *) afs_vop_open},	/* open */
-    {&vop_pathconf_desc, (vop_t *) afs_vop_pathconf},	/* pathconf */
-    {&vop_poll_desc, (vop_t *) vop_nopoll},	/* select */
-    {&vop_print_desc, (vop_t *) afs_vop_print},	/* print */
-    {&vop_read_desc, (vop_t *) afs_vop_read},	/* read */
-    {&vop_readdir_desc, (vop_t *) afs_vop_readdir},	/* readdir */
-    {&vop_readlink_desc, (vop_t *) afs_vop_readlink},	/* readlink */
-    {&vop_reclaim_desc, (vop_t *) afs_vop_reclaim},	/* reclaim */
-    {&vop_remove_desc, (vop_t *) afs_vop_remove},	/* remove */
-    {&vop_rename_desc, (vop_t *) afs_vop_rename},	/* rename */
-    {&vop_rmdir_desc, (vop_t *) afs_vop_rmdir},	/* rmdir */
-    {&vop_setattr_desc, (vop_t *) afs_vop_setattr},	/* setattr */
-    {&vop_strategy_desc, (vop_t *) afs_vop_strategy},	/* strategy */
-    {&vop_symlink_desc, (vop_t *) afs_vop_symlink},	/* symlink */
-    {&vop_write_desc, (vop_t *) afs_vop_write},	/* write */
-    {&vop_ioctl_desc, (vop_t *) afs_vop_ioctl},	/* XXX ioctl */
-    /*{ &vop_seek_desc, afs_vop_seek }, *//* seek */
-#if defined(AFS_FBSD70_ENV) && !defined(AFS_FBSD90_ENV)
-    {&vop_lock1_desc, (vop_t *) afs_vop_lock}, /* lock */
-    {&vop_unlock_desc, (vop_t *) afs_vop_unlock}, /* unlock */
-    {&vop_islocked_desc, (vop_t *) afs_vop_islocked}, /* islocked */
-#endif
-    {NULL, NULL}
-};
-struct vnodeopv_desc afs_vnodeop_opv_desc =
-    { &afs_vnodeop_p, afs_vnodeop_entries };
-#endif /* AFS_FBSD60_ENV */
-
 #define GETNAME()       \
     struct componentname *cnp = ap->a_cnp; \
     char *name; \
-    MALLOC(name, char *, cnp->cn_namelen+1, M_TEMP, M_WAITOK); \
+    name = malloc(cnp->cn_namelen+1, M_TEMP, M_WAITOK); \
     memcpy(name, cnp->cn_nameptr, cnp->cn_namelen); \
     name[cnp->cn_namelen] = '\0'
 
-#define DROPNAME() FREE(name, M_TEMP)
+#define DROPNAME() free(name, M_TEMP)
 
+#ifdef LINK_MAX
+# define AFS_LINK_MAX LINK_MAX
+#else
+# define AFS_LINK_MAX (32767)
+#endif
+
 /*
  * Here we define compatibility functions/macros for interfaces that
  * have changed between different FreeBSD versions.
  */
-#if defined(AFS_FBSD90_ENV)
 static __inline void ma_vm_page_lock_queues(void) {};
 static __inline void ma_vm_page_unlock_queues(void) {};
 static __inline void ma_vm_page_lock(vm_page_t m) { vm_page_lock(m); };
 static __inline void ma_vm_page_unlock(vm_page_t m) { vm_page_unlock(m); };
-#else
-static __inline void ma_vm_page_lock_queues(void) { vm_page_lock_queues(); };
-static __inline void ma_vm_page_unlock_queues(void) { vm_page_unlock_queues(); };
-static __inline void ma_vm_page_lock(vm_page_t m) {};
-static __inline void ma_vm_page_unlock(vm_page_t m) {};
-#endif
 
-#if defined(AFS_FBSD80_ENV)
-#define ma_vn_lock(vp, flags, p) (vn_lock(vp, flags))
-#define MA_VOP_LOCK(vp, flags, p) (VOP_LOCK(vp, flags))
-#define MA_VOP_UNLOCK(vp, flags, p) (VOP_UNLOCK(vp, flags))
-#else
-#define ma_vn_lock(vp, flags, p) (vn_lock(vp, flags, p))
-#define MA_VOP_LOCK(vp, flags, p) (VOP_LOCK(vp, flags, p))
-#define MA_VOP_UNLOCK(vp, flags, p) (VOP_UNLOCK(vp, flags, p))
-#endif
-
-#if defined(AFS_FBSD70_ENV)
-#define MA_PCPU_INC(c) PCPU_INC(c)
-#define	MA_PCPU_ADD(c, n) PCPU_ADD(c, n)
-#else
-#define MA_PCPU_INC(c) PCPU_LAZY_INC(c)
-#define	MA_PCPU_ADD(c, n) (c) += (n)
-#endif
-
 #if __FreeBSD_version >= 1000030
 #define AFS_VM_OBJECT_WLOCK(o)	VM_OBJECT_WLOCK(o)
 #define AFS_VM_OBJECT_WUNLOCK(o)	VM_OBJECT_WUNLOCK(o)
@@ -274,113 +101,14 @@ static __inline void ma_vm_page_unlock(vm_page_t m) {}
 #define AFS_VM_OBJECT_WUNLOCK(o)	VM_OBJECT_UNLOCK(o)
 #endif
 
-#ifdef AFS_FBSD70_ENV
-#ifndef AFS_FBSD80_ENV
-/* From kern_lock.c */
-#define	COUNT(td, x)	if ((td)) (td)->td_locks += (x)
-#define LK_ALL (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE | \
-	LK_SHARE_NONZERO | LK_WAIT_NONZERO)
-
-static __inline void
-sharelock(struct thread *td, struct lock *lkp, int incr) {
-	lkp->lk_flags |= LK_SHARE_NONZERO;
-	lkp->lk_sharecount += incr;
-	COUNT(td, incr);
-}
-#endif
-
-/*
- * Standard lock, unlock and islocked functions.
- */
-int
-afs_vop_lock(ap)
-    struct vop_lock1_args /* {
-			     struct vnode *a_vp;
-			     int a_flags;
-			     struct thread *a_td;
-			     char *file;
-			     int line;
-			     } */ *ap;
-{
-    struct vnode *vp = ap->a_vp;
-    struct lock *lkp = vp->v_vnlock;
-
-#if 0 && defined(AFS_FBSD80_ENV) && !defined(UKERNEL)
-    afs_warn("afs_vop_lock: tid %d pid %d \"%s\"\n", curthread->td_tid,
-	     curthread->td_proc->p_pid, curthread->td_name);
-    kdb_backtrace();
-#endif
-
-#ifdef AFS_FBSD80_ENV
-    return (_lockmgr_args(lkp, ap->a_flags, VI_MTX(vp),
-			  LK_WMESG_DEFAULT, LK_PRIO_DEFAULT, LK_TIMO_DEFAULT,
-			  ap->a_file, ap->a_line));
+#ifdef VM_CNT_ADD
+# define AFS_VM_CNT_ADD(var, x) VM_CNT_ADD(var, x)
+# define AFS_VM_CNT_INC(var)    VM_CNT_INC(var)
 #else
-    return (_lockmgr(lkp, ap->a_flags, VI_MTX(vp), ap->a_td, ap->a_file, ap->a_line));
+# define AFS_VM_CNT_ADD(var, x) PCPU_ADD(cnt.var, x)
+# define AFS_VM_CNT_INC(var)    PCPU_INC(cnt.var)
 #endif
-}
 
-/* See above. */
-int
-afs_vop_unlock(ap)
-    struct vop_unlock_args /* {
-			      struct vnode *a_vp;
-			      int a_flags;
-			      struct thread *a_td;
-			      } */ *ap;
-{
-    struct vnode *vp = ap->a_vp;
-    struct lock *lkp = vp->v_vnlock;
-
-#ifdef AFS_FBSD80_ENV
-    int code = 0;
-    u_int op;
-    op = ((ap->a_flags) | LK_RELEASE) & LK_TYPE_MASK;
-    int glocked = ISAFS_GLOCK();
-    if (glocked)
-	AFS_GUNLOCK();
-    if ((op & (op - 1)) != 0) {
-      afs_warn("afs_vop_unlock: Shit.\n");
-      goto done;
-    }
-    code = lockmgr(lkp, ap->a_flags | LK_RELEASE, VI_MTX(vp));
- done:
-    if (glocked)
-	AFS_GLOCK();
-    return(code);
-#else
-    /* possibly in current code path where this
-     * forces trace, we should have had a (shared? not
-     * necessarily, see _lockmgr in kern_lock.c) lock
-     * and that's the real bug.  but. 
-     */
-    critical_enter();
-    if ((lkp->lk_exclusivecount == 0) &&
-	(!(lkp->lk_flags & LK_SHARE_NONZERO))) {
-	sharelock(ap->a_td, lkp, 1);
-    }
-    critical_exit();
-    return (lockmgr(lkp, ap->a_flags | LK_RELEASE, VI_MTX(vp),
-		    ap->a_td));
-#endif
-}
-
-/* See above. */
-int
-afs_vop_islocked(ap)
-    struct vop_islocked_args /* {
-				struct vnode *a_vp;
-				struct thread *a_td; (not in 80)
-				} */ *ap;
-{
-#ifdef AFS_FBSD80_ENV
-    return (lockstatus(ap->a_vp->v_vnlock));
-#else
-    return (lockstatus(ap->a_vp->v_vnlock, ap->a_td));
-#endif
-}
-#endif /* 70 */
-
 /*
  * Mosty copied from sys/ufs/ufs/ufs_vnops.c:ufs_pathconf().
  * We should know the correct answers to these questions with
@@ -395,7 +123,7 @@ afs_vop_pathconf(struct vop_pathconf_args *ap)
 	error = 0;
 	switch (ap->a_name) {
 	case _PC_LINK_MAX:
-		*ap->a_retval = LINK_MAX;
+		*ap->a_retval = AFS_LINK_MAX;
 		break;
 	case _PC_NAME_MAX:
 		*ap->a_retval = NAME_MAX;
@@ -474,7 +202,7 @@ afs_vop_pathconf(struct vop_pathconf_args *ap)
 	return (error);
 }
 
-int
+static int
 afs_vop_lookup(ap)
      struct vop_lookup_args	/* {
 				 * struct vnodeop_desc * a_desc;
@@ -487,17 +215,9 @@ afs_vop_lookup(ap)
     struct vcache *vcp;
     struct vnode *vp, *dvp;
     int flags = ap->a_cnp->cn_flags;
-    int lockparent;		/* 1 => lockparent flag is set */
-    int wantparent;		/* 1 => wantparent or lockparent flag */
-#ifndef AFS_FBSD80_ENV
-    struct thread *p = ap->a_cnp->cn_thread;
-#endif
 
     dvp = ap->a_dvp;
     if (dvp->v_type != VDIR) {
-#ifndef AFS_FBSD70_ENV
-	*ap->a_vpp = 0;
-#endif
 	return ENOTDIR;
     }
 
@@ -506,15 +226,34 @@ afs_vop_lookup(ap)
 
     GETNAME();
 
-    lockparent = flags & LOCKPARENT;
-    wantparent = flags & (LOCKPARENT | WANTPARENT);
-
 #if __FreeBSD_version < 1000021
     cnp->cn_flags |= MPSAFE; /* steel */
 #endif
 
+    /*
+     * Locking rules:
+     *
+     * - 'dvp' is locked by our caller. We must return it locked, whether we
+     * return success or error.
+     *
+     * - If the lookup succeeds, 'vp' must be locked before we return.
+     *
+     * - If we lock multiple vnodes, parent vnodes must be locked before
+     * children vnodes.
+     *
+     * As a result, looking up the parent directory (if 'flags' has ISDOTDOT
+     * set) is a bit of a special case. In that case, we must unlock 'dvp'
+     * before performing the lookup, since the lookup operation may lock the
+     * target vnode, and the target vnode is the parent of 'dvp' (so we must
+     * lock 'dvp' after locking the target vnode).
+     */
+
     if (flags & ISDOTDOT)
-	MA_VOP_UNLOCK(dvp, 0, p);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(dvp);
+#else
+	VOP_UNLOCK(dvp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
 
     AFS_GLOCK();
     error = afs_lookup(VTOAFS(dvp), name, &vcp, cnp->cn_cred);
@@ -522,7 +261,7 @@ afs_vop_lookup(ap)
 
     if (error) {
 	if (flags & ISDOTDOT)
-	    MA_VOP_LOCK(dvp, LK_EXCLUSIVE | LK_RETRY, p);
+	    vn_lock(dvp, LK_EXCLUSIVE | LK_RETRY);
 	if ((cnp->cn_nameiop == CREATE || cnp->cn_nameiop == RENAME)
 	    && (flags & ISLASTCN) && error == ENOENT)
 	    error = EJUSTRETURN;
@@ -534,44 +273,26 @@ afs_vop_lookup(ap)
     }
     vp = AFSTOV(vcp);		/* always get a node if no error */
 
-    /* The parent directory comes in locked.  We unlock it on return
-     * unless the caller wants it left locked.
-     * we also always return the vnode locked. */
-
     if (flags & ISDOTDOT) {
-	/* vp before dvp since we go root to leaf, and .. comes first */
-	ma_vn_lock(vp, LK_EXCLUSIVE | LK_RETRY, p);
-	ma_vn_lock(dvp, LK_EXCLUSIVE | LK_RETRY, p);
-	/* always return the child locked */
-	if (lockparent && (flags & ISLASTCN)
-	    && (error = ma_vn_lock(dvp, LK_EXCLUSIVE, p))) {
-	    vput(vp);
-	    DROPNAME();
-	    return (error);
-	}
+	/* Must lock 'vp' before 'dvp', since 'vp' is the parent vnode. */
+	vn_lock(vp, LK_EXCLUSIVE | LK_RETRY);
+	vn_lock(dvp, LK_EXCLUSIVE | LK_RETRY);
     } else if (vp == dvp) {
 	/* they're the same; afs_lookup() already ref'ed the leaf.
 	 * It came in locked, so we don't need to ref OR lock it */
     } else {
-	if (!lockparent || !(flags & ISLASTCN)) {
-#ifndef AFS_FBSD70_ENV /* 6 too? */
-	    MA_VOP_UNLOCK(dvp, 0, p);	/* done with parent. */
-#endif
-	}
-	ma_vn_lock(vp, LK_EXCLUSIVE | LK_CANRECURSE | LK_RETRY, p);
-	/* always return the child locked */
+	vn_lock(vp, LK_EXCLUSIVE | LK_CANRECURSE | LK_RETRY);
     }
     *ap->a_vpp = vp;
 
-    if ((cnp->cn_nameiop == RENAME && wantparent && (flags & ISLASTCN))
-	|| (cnp->cn_nameiop != LOOKUP && (flags & ISLASTCN)))
+    if (cnp->cn_nameiop != LOOKUP && (flags & ISLASTCN))
 	cnp->cn_flags |= SAVENAME;
 
     DROPNAME();
     return error;
 }
 
-int
+static int
 afs_vop_create(ap)
      struct vop_create_args	/* {
 				 * struct vnode *a_dvp;
@@ -583,9 +304,6 @@ afs_vop_create(ap)
     int error = 0;
     struct vcache *vcp;
     struct vnode *dvp = ap->a_dvp;
-#ifndef AFS_FBSD80_ENV
-    struct thread *p = ap->a_cnp->cn_thread;
-#endif
     GETNAME();
 
     AFS_GLOCK();
@@ -601,7 +319,7 @@ afs_vop_create(ap)
 
     if (vcp) {
 	*ap->a_vpp = AFSTOV(vcp);
-	ma_vn_lock(AFSTOV(vcp), LK_EXCLUSIVE | LK_RETRY, p);
+	vn_lock(AFSTOV(vcp), LK_EXCLUSIVE | LK_RETRY);
     } else
 	*ap->a_vpp = 0;
 
@@ -609,7 +327,7 @@ afs_vop_create(ap)
     return error;
 }
 
-int
+static int
 afs_vop_mknod(ap)
      struct vop_mknod_args	/* {
 				 * struct vnode *a_dvp;
@@ -621,28 +339,7 @@ afs_vop_mknod(ap)
     return (ENODEV);
 }
 
-#if 0
 static int
-validate_vops(struct vnode *vp, int after)
-{
-    int ret = 0;
-    struct vnodeopv_entry_desc *this;
-    for (this = afs_vnodeop_entries; this->opve_op; this++) {
-	if (vp->v_op[this->opve_op->vdesc_offset] != this->opve_impl) {
-	    if (!ret) {
-		printf("v_op %d ", after);
-		vprint("check", vp);
-	    }
-	    ret = 1;
-	    printf("For oper %d (%s), func is %p, not %p",
-		   this->opve_op->vdesc_offset, this->opve_op->vdesc_name,
-		   vp->v_op[this->opve_op->vdesc_offset], this->opve_impl);
-	}
-    }
-    return ret;
-}
-#endif
-int
 afs_vop_open(ap)
      struct vop_open_args	/* {
 				 * struct vnode *a_vp;
@@ -662,14 +359,12 @@ afs_vop_open(ap)
 	panic("AFS open changed vnode!");
 #endif
     AFS_GUNLOCK();
-#ifdef AFS_FBSD60_ENV
     vnode_create_vobject(ap->a_vp, vc->f.m.Length, ap->a_td);
-#endif
     osi_FlushPages(vc, ap->a_cred);
     return error;
 }
 
-int
+static int
 afs_vop_close(ap)
      struct vop_close_args	/* {
 				 * struct vnode *a_vp;
@@ -678,15 +373,14 @@ afs_vop_close(ap)
 				 * struct thread *a_td;
 				 * } */ *ap;
 {
-    int code, iflag;
+    int code, doomed;
     struct vnode *vp = ap->a_vp;
     struct vcache *avc = VTOAFS(vp);
 
-#if defined(AFS_FBSD80_ENV)
     VI_LOCK(vp);
-    iflag = vp->v_iflag & VI_DOOMED;
+    doomed = VN_IS_DOOMED(vp);
     VI_UNLOCK(vp);
-    if (iflag & VI_DOOMED) {
+    if (doomed) {
         /* osi_FlushVCache (correctly) calls vgone() on recycled vnodes, we don't
          * have an afs_close to process, in that case */
         if (avc->opens != 0)
@@ -694,7 +388,6 @@ afs_vop_close(ap)
                   vp, avc, avc->opens);
         return 0;
     }
-#endif
 
     AFS_GLOCK();
     if (ap->a_cred)
@@ -702,11 +395,19 @@ afs_vop_close(ap)
     else
 	code = afs_close(avc, ap->a_fflag, afs_osi_credp);
     osi_FlushPages(avc, ap->a_cred);	/* hold GLOCK, but not basic vnode lock */
+
+    ObtainWriteLock(&avc->lock, 808);
+    if (avc->cred != NULL) {
+	crfree(avc->cred);
+	avc->cred = NULL;
+    }
+    ReleaseWriteLock(&avc->lock);
+
     AFS_GUNLOCK();
     return code;
 }
 
-int
+static int
 afs_vop_access(ap)
      struct vop_access_args	/* {
 				 * struct vnode *a_vp;
@@ -717,16 +418,12 @@ afs_vop_access(ap)
 {
     int code;
     AFS_GLOCK();
-#if defined(AFS_FBSD80_ENV)
     code = afs_access(VTOAFS(ap->a_vp), ap->a_accmode, ap->a_cred);
-#else
-    code = afs_access(VTOAFS(ap->a_vp), ap->a_mode, ap->a_cred);
-#endif
     AFS_GUNLOCK();
     return code;
 }
 
-int
+static int
 afs_vop_getattr(ap)
      struct vop_getattr_args	/* {
 				 * struct vnode *a_vp;
@@ -743,7 +440,7 @@ afs_vop_getattr(ap)
     return code;
 }
 
-int
+static int
 afs_vop_setattr(ap)
      struct vop_setattr_args	/* {
 				 * struct vnode *a_vp;
@@ -758,7 +455,7 @@ afs_vop_setattr(ap)
     return code;
 }
 
-int
+static int
 afs_vop_read(ap)
      struct vop_read_args	/* {
 				 * struct vnode *a_vp;
@@ -785,7 +482,7 @@ afs_vop_read(ap)
  *	int *a_rahead;
  * };
  */
-int
+static int
 afs_vop_getpages(struct vop_getpages_args *ap)
 {
     int code;
@@ -858,12 +555,16 @@ afs_vop_getpages(struct vop_getpages_args *ap)
 	ma_vm_page_unlock_queues();
 	AFS_VM_OBJECT_WUNLOCK(object);
     }
+#if defined(AFS_FBSD_UMA_BUFS)
+    bp = uma_zalloc(afs_pbuf_zone, M_WAITOK);
+#else
     bp = getpbuf(&afs_pbuf_freecnt);
+#endif
 
     kva = (vm_offset_t) bp->b_data;
     pmap_qenter(kva, pages, npages);
-    MA_PCPU_INC(cnt.v_vnodein);
-    MA_PCPU_ADD(cnt.v_vnodepgsin, npages);
+    AFS_VM_CNT_INC(v_vnodein);
+    AFS_VM_CNT_ADD(v_vnodepgsin, npages);
 
 #ifdef FBSD_VOP_GETPAGES_BUSIED
     count = ctob(npages);
@@ -886,7 +587,11 @@ afs_vop_getpages(struct vop_getpages_args *ap)
     AFS_GUNLOCK();
     pmap_qremove(kva, npages);
 
+#if defined(AFS_FBSD_UMA_BUFS)
+    uma_zfree(afs_pbuf_zone, bp);
+#else
     relpbuf(bp, &afs_pbuf_freecnt);
+#endif
 
     if (code && (uio.uio_resid == count)) {
 #ifndef FBSD_VOP_GETPAGES_BUSIED
@@ -918,11 +623,7 @@ afs_vop_getpages(struct vop_getpages_args *ap)
 	     * Read operation filled an entire page
 	     */
 	    m->valid = VM_PAGE_BITS_ALL;
-#ifndef AFS_FBSD80_ENV
-	    vm_page_undirty(m);
-#else
 	    KASSERT(m->dirty == 0, ("afs_getpages: page %p is dirty", m));
-#endif
 	} else if (size > toff) {
 	    /*
 	     * Read operation filled a partial page.
@@ -950,11 +651,7 @@ afs_vop_getpages(struct vop_getpages_args *ap)
 	     * now tell them that it is ok to use.
 	     */
 	    if (!code) {
-#if defined(AFS_FBSD70_ENV)
 		if (m->oflags & VPO_WANTED) {
-#else
-		if (m->flags & PG_WANTED) {
-#endif
 		    ma_vm_page_lock(m);
 		    vm_page_activate(m);
 		    ma_vm_page_unlock(m);
@@ -979,7 +676,7 @@ afs_vop_getpages(struct vop_getpages_args *ap)
     return VM_PAGER_OK;
 }
 
-int
+static int
 afs_vop_write(ap)
      struct vop_write_args	/* {
 				 * struct vnode *a_vp;
@@ -990,11 +687,19 @@ afs_vop_write(ap)
 {
     int code;
     struct vcache *avc = VTOAFS(ap->a_vp);
+    off_t start, end;
+    start = AFS_UIO_OFFSET(ap->a_uio);
+    end = start + AFS_UIO_RESID(ap->a_uio);
+
     AFS_GLOCK();
     osi_FlushPages(avc, ap->a_cred);	/* hold GLOCK, but not basic vnode lock */
     code =
 	afs_write(VTOAFS(ap->a_vp), ap->a_uio, ap->a_ioflag, ap->a_cred, 0);
     AFS_GUNLOCK();
+
+    /* Invalidate any pages in the written area. */
+    vn_pages_remove(ap->a_vp, OFF_TO_IDX(start), OFF_TO_IDX(end));
+
     return code;
 }
 
@@ -1012,7 +717,7 @@ afs_vop_write(ap)
  * All of the pages passed to us in ap->a_m[] are already marked as busy,
  * so there is no additional locking required to set their flags.  -GAW
  */
-int
+static int
 afs_vop_putpages(struct vop_putpages_args *ap)
 {
     int code;
@@ -1023,6 +728,7 @@ afs_vop_putpages(struct vop_putpages_args *ap)
     vm_offset_t kva;
     struct vnode *vp;
     struct vcache *avc;
+    struct ucred *cred;
 
     memset(&uio, 0, sizeof(uio));
     memset(&iov, 0, sizeof(iov));
@@ -1041,12 +747,16 @@ afs_vop_putpages(struct vop_putpages_args *ap)
     npages = btoc(ap->a_count);
     for (i = 0; i < npages; i++)
 	ap->a_rtvals[i] = VM_PAGER_AGAIN;
+#if defined(AFS_FBSD_UMA_BUFS)
+    bp = uma_zalloc(afs_pbuf_zone, M_WAITOK);
+#else
     bp = getpbuf(&afs_pbuf_freecnt);
+#endif
 
     kva = (vm_offset_t) bp->b_data;
     pmap_qenter(kva, ap->a_m, npages);
-    MA_PCPU_INC(cnt.v_vnodeout);
-    MA_PCPU_ADD(cnt.v_vnodepgsout, ap->a_count);
+    AFS_VM_CNT_INC(v_vnodeout);
+    AFS_VM_CNT_ADD(v_vnodepgsout, ap->a_count);
 
     iov.iov_base = (caddr_t) kva;
     iov.iov_len = ap->a_count;
@@ -1064,23 +774,45 @@ afs_vop_putpages(struct vop_putpages_args *ap)
      * sync |= IO_INVAL; */
 
     AFS_GLOCK();
-    code = afs_write(avc, &uio, sync, osi_curcred(), 0);
+
+    ObtainReadLock(&avc->lock);
+    if (avc->cred != NULL) {
+	/*
+	 * Use the creds from the process that opened this file for writing; if
+	 * any. Otherwise, if we use the current process's creds, we may use
+	 * the creds for uid 0 if we are writing back pages from the syncer(4)
+	 * process.
+	 */
+	cred = crhold(avc->cred);
+    } else {
+	cred = crhold(curthread->td_ucred);
+    }
+    ReleaseReadLock(&avc->lock);
+
+    code = afs_write(avc, &uio, sync, cred, 0);
     AFS_GUNLOCK();
 
     pmap_qremove(kva, npages);
+#if defined(AFS_FBSD_UMA_BUFS)
+    uma_zfree(afs_pbuf_zone, bp);
+#else
     relpbuf(bp, &afs_pbuf_freecnt);
+#endif
 
     if (!code) {
+	AFS_VM_OBJECT_WLOCK(vp->v_object);
 	size = ap->a_count - uio.uio_resid;
 	for (i = 0; i < round_page(size) / PAGE_SIZE; i++) {
 	    ap->a_rtvals[i] = VM_PAGER_OK;
 	    vm_page_undirty(ap->a_m[i]);
 	}
+	AFS_VM_OBJECT_WUNLOCK(vp->v_object);
     }
+    crfree(cred);
     return ap->a_rtvals[0];
 }
 
-int
+static int
 afs_vop_ioctl(ap)
      struct vop_ioctl_args	/* {
 				 * struct vnode *a_vp;
@@ -1109,7 +841,7 @@ afs_vop_ioctl(ap)
     }
 }
 
-int
+static int
 afs_vop_fsync(ap)
      struct vop_fsync_args	/* {
 				 * struct vnode *a_vp;
@@ -1122,19 +854,12 @@ afs_vop_fsync(ap)
 
     AFS_GLOCK();
     /*vflushbuf(vp, wait); */
-#ifdef AFS_FBSD60_ENV
     error = afs_fsync(VTOAFS(vp), ap->a_td->td_ucred);
-#else
-    if (ap->a_cred)
-	error = afs_fsync(VTOAFS(vp), ap->a_cred);
-    else
-	error = afs_fsync(VTOAFS(vp), afs_osi_credp);
-#endif
     AFS_GUNLOCK();
     return error;
 }
 
-int
+static int
 afs_vop_remove(ap)
      struct vop_remove_args	/* {
 				 * struct vnode *a_dvp;
@@ -1155,7 +880,7 @@ afs_vop_remove(ap)
     return error;
 }
 
-int
+static int
 afs_vop_link(ap)
      struct vop_link_args	/* {
 				 * struct vnode *a_vp;
@@ -1166,9 +891,6 @@ afs_vop_link(ap)
     int error = 0;
     struct vnode *dvp = ap->a_tdvp;
     struct vnode *vp = ap->a_vp;
-#ifndef AFS_FBSD80_ENV
-    struct thread *p = ap->a_cnp->cn_thread;
-#endif
 
     GETNAME();
     if (dvp->v_mount != vp->v_mount) {
@@ -1179,20 +901,24 @@ afs_vop_link(ap)
 	error = EISDIR;
 	goto out;
     }
-    if ((error = ma_vn_lock(vp, LK_CANRECURSE | LK_EXCLUSIVE, p)) != 0) {
+    if ((error = vn_lock(vp, LK_CANRECURSE | LK_EXCLUSIVE)) != 0) {
 	goto out;
     }
     AFS_GLOCK();
     error = afs_link(VTOAFS(vp), VTOAFS(dvp), name, cnp->cn_cred);
     AFS_GUNLOCK();
     if (dvp != vp)
-	MA_VOP_UNLOCK(vp, 0, p);
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+	VOP_UNLOCK(vp);
+#else
+	VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
   out:
     DROPNAME();
     return error;
 }
 
-int
+static int
 afs_vop_rename(ap)
      struct vop_rename_args	/* {
 				 * struct vnode *a_fdvp;
@@ -1212,9 +938,6 @@ afs_vop_rename(ap)
     struct vnode *tdvp = ap->a_tdvp;
     struct vnode *fvp = ap->a_fvp;
     struct vnode *fdvp = ap->a_fdvp;
-#ifndef AFS_FBSD80_ENV
-    struct thread *p = fcnp->cn_thread;
-#endif
 
     /*
      * Check for cross-device rename.
@@ -1273,13 +996,13 @@ afs_vop_rename(ap)
 	vput(fvp);
 	return (error);
     }
-    if ((error = ma_vn_lock(fvp, LK_EXCLUSIVE, p)) != 0)
+    if ((error = vn_lock(fvp, LK_EXCLUSIVE)) != 0)
 	goto abortit;
 
-    MALLOC(fname, char *, fcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
+    fname = malloc(fcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
     memcpy(fname, fcnp->cn_nameptr, fcnp->cn_namelen);
     fname[fcnp->cn_namelen] = '\0';
-    MALLOC(tname, char *, tcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
+    tname = malloc(tcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
     memcpy(tname, tcnp->cn_nameptr, tcnp->cn_namelen);
     tname[tcnp->cn_namelen] = '\0';
 
@@ -1290,8 +1013,8 @@ afs_vop_rename(ap)
 	afs_rename(VTOAFS(fdvp), fname, VTOAFS(tdvp), tname, tcnp->cn_cred);
     AFS_GUNLOCK();
 
-    FREE(fname, M_TEMP);
-    FREE(tname, M_TEMP);
+    free(fname, M_TEMP);
+    free(tname, M_TEMP);
     if (tdvp == tvp)
 	vrele(tdvp);
     else
@@ -1303,7 +1026,7 @@ afs_vop_rename(ap)
     return error;
 }
 
-int
+static int
 afs_vop_mkdir(ap)
      struct vop_mkdir_args	/* {
 				 * struct vnode *a_dvp;
@@ -1316,9 +1039,6 @@ afs_vop_mkdir(ap)
     struct vattr *vap = ap->a_vap;
     int error = 0;
     struct vcache *vcp;
-#ifndef AFS_FBSD80_ENV
-    struct thread *p = ap->a_cnp->cn_thread;
-#endif
 
     GETNAME();
 #ifdef DIAGNOSTIC
@@ -1334,14 +1054,14 @@ afs_vop_mkdir(ap)
     }
     if (vcp) {
 	*ap->a_vpp = AFSTOV(vcp);
-	ma_vn_lock(AFSTOV(vcp), LK_EXCLUSIVE | LK_RETRY, p);
+	vn_lock(AFSTOV(vcp), LK_EXCLUSIVE | LK_RETRY);
     } else
 	*ap->a_vpp = 0;
     DROPNAME();
     return error;
 }
 
-int
+static int
 afs_vop_rmdir(ap)
      struct vop_rmdir_args	/* {
 				 * struct vnode *a_dvp;
@@ -1368,7 +1088,7 @@ afs_vop_rmdir(ap)
  *	char *a_target;
  * };
  */
-int
+static int
 afs_vop_symlink(struct vop_symlink_args *ap)
 {
     struct vnode *dvp;
@@ -1382,23 +1102,24 @@ afs_vop_symlink(struct vop_symlink_args *ap)
     dvp = ap->a_dvp;
     newvp = NULL;
 
+    // __DECONST needed as of 1493c2ee62b8
     error =
-	afs_symlink(VTOAFS(dvp), name, ap->a_vap, ap->a_target, NULL,
+	afs_symlink(VTOAFS(dvp), name, ap->a_vap, __DECONST(char*, ap->a_target), NULL,
 		    cnp->cn_cred);
     if (error == 0) {
 	error = afs_lookup(VTOAFS(dvp), name, &vcp, cnp->cn_cred);
-	if (error == 0) {
-	    newvp = AFSTOV(vcp);
-	    ma_vn_lock(newvp, LK_EXCLUSIVE | LK_RETRY, cnp->cn_thread);
-	}
     }
     AFS_GUNLOCK();
+    if (error == 0) {
+	newvp = AFSTOV(vcp);
+	vn_lock(newvp, LK_EXCLUSIVE | LK_RETRY);
+    }
     DROPNAME();
     *(ap->a_vpp) = newvp;
     return error;
 }
 
-int
+static int
 afs_vop_readdir(ap)
      struct vop_readdir_args	/* {
 				 * struct vnode *a_vp;
@@ -1434,7 +1155,7 @@ afs_vop_readdir(ap)
 	     dp = (const struct dirent *)((const char *)dp + dp->d_reclen))
 	    ncookies++;
 
-	MALLOC(cookies, u_long *, ncookies * sizeof(u_long), M_TEMP,
+	cookies = malloc(ncookies * sizeof(u_long), M_TEMP,
 	       M_WAITOK);
 	for (dp = dp_start, cookiep = cookies; dp < dp_end;
 	     dp = (const struct dirent *)((const char *)dp + dp->d_reclen)) {
@@ -1448,7 +1169,7 @@ afs_vop_readdir(ap)
     return error;
 }
 
-int
+static int
 afs_vop_readlink(ap)
      struct vop_readlink_args	/* {
 				 * struct vnode *a_vp;
@@ -1464,7 +1185,7 @@ afs_vop_readlink(ap)
     return error;
 }
 
-int
+static int
 afs_vop_inactive(ap)
      struct vop_inactive_args	/* {
 				 * struct vnode *a_vp;
@@ -1476,9 +1197,6 @@ afs_vop_inactive(ap)
     AFS_GLOCK();
     afs_InactiveVCache(VTOAFS(vp), 0);	/* decrs ref counts */
     AFS_GUNLOCK();
-#ifndef AFS_FBSD60_ENV
-    MA_VOP_UNLOCK(vp, 0, ap->a_td);
-#endif
     return 0;
 }
 
@@ -1487,21 +1205,40 @@ afs_vop_inactive(ap)
  *	struct vnode *a_vp;
  * };
  */
-int
+static int
 afs_vop_reclaim(struct vop_reclaim_args *ap)
 {
-    /* copied from ../OBSD/osi_vnodeops.c:afs_nbsd_reclaim() */
     int code, slept;
     struct vnode *vp = ap->a_vp;
     struct vcache *avc = VTOAFS(vp);
     int haveGlock = ISAFS_GLOCK();
-    int haveVlock = CheckLock(&afs_xvcache);
 
+    /*
+     * In other code paths, we acquire the vnode lock while afs_xvcache is
+     * already held (e.g. afs_PutVCache() -> vrele()). Here, we already have
+     * the vnode lock, and we need afs_xvcache. So drop the vnode lock in order
+     * to hold afs_xvcache.
+     */
+#if defined(AFS_FBSD_VOP_UNLOCK_NOFLAGS)
+    VOP_UNLOCK(vp);
+#else
+    VOP_UNLOCK(vp, 0);
+#endif /* AFS_FBSD_VOP_UNLOCK_NOFLAGS */
+
     if (!haveGlock)
 	AFS_GLOCK();
-    if (!haveVlock)
-	ObtainWriteLock(&afs_xvcache, 901);
-    /* reclaim the vnode and the in-memory vcache, but keep the on-disk vcache */
+    ObtainWriteLock(&afs_xvcache, 901);
+
+    /*
+     * Note that we deliberately call VOP_LOCK() instead of vn_lock() here.
+     * vn_lock() will return an error for VI_DOOMED vnodes, but we know this
+     * vnode is already VI_DOOMED. We just want to lock it again, and skip the
+     * VI_DOOMED check.
+     */
+    AFS_GUNLOCK();
+    VOP_LOCK(vp, LK_EXCLUSIVE);
+    AFS_GLOCK();
+
     code = afs_FlushVCache(avc, &slept);
 
     if (avc->f.states & CVInit) {
@@ -1509,51 +1246,23 @@ afs_vop_reclaim(struct vop_reclaim_args *ap)
 	afs_osi_Wakeup(&avc->f.states);
     }
 
-    if (!haveVlock)
-	ReleaseWriteLock(&afs_xvcache);
+    ReleaseWriteLock(&afs_xvcache);
     if (!haveGlock)
 	AFS_GUNLOCK();
 
     if (code) {
 	afs_warn("afs_vop_reclaim: afs_FlushVCache failed code %d vnode\n", code);
 	VOP_PRINT(vp);
+	panic("afs: afs_FlushVCache failed during reclaim");
     }
 
-    /* basically, it must not fail */
     vnode_destroy_vobject(vp);
     vp->v_data = 0;
 
     return 0;
 }
 
-#ifndef AFS_FBSD60_ENV
-int
-afs_vop_bmap(ap)
-     struct vop_bmap_args	/* {
-				 * struct vnode *a_vp;
-				 * daddr_t  a_bn;
-				 * struct vnode **a_vpp;
-				 * daddr_t *a_bnp;
-				 * int *a_runp;
-				 * int *a_runb;
-				 * } */ *ap;
-{
-    if (ap->a_bnp) {
-	*ap->a_bnp = ap->a_bn * (PAGE_SIZE / DEV_BSIZE);
-    }
-    if (ap->a_vpp) {
-	*ap->a_vpp = ap->a_vp;
-    }
-    if (ap->a_runp != NULL)
-	*ap->a_runp = 0;
-    if (ap->a_runb != NULL)
-	*ap->a_runb = 0;
-
-    return 0;
-}
-#endif
-
-int
+static int
 afs_vop_strategy(ap)
      struct vop_strategy_args	/* {
 				 * struct buf *a_bp;
@@ -1566,7 +1275,7 @@ afs_vop_strategy(ap)
     return error;
 }
 
-int
+static int
 afs_vop_print(ap)
      struct vop_print_args	/* {
 				 * struct vnode *a_vp;
@@ -1576,7 +1285,7 @@ afs_vop_print(ap)
     struct vcache *vc = VTOAFS(ap->a_vp);
     int s = vc->f.states;
 
-    printf("vc %p vp %p tag %s, fid: %d.%d.%d.%d, opens %d, writers %d", vc, vp, vp->v_tag,
+    printf("vc %p vp %p, fid: %d.%d.%d.%d, opens %d, writers %d", vc, vp,
 	   (int)vc->f.fid.Cell, (u_int) vc->f.fid.Fid.Volume,
 	   (u_int) vc->f.fid.Fid.Vnode, (u_int) vc->f.fid.Fid.Unique, vc->opens,
 	   vc->execsOrWriters);
@@ -1591,7 +1300,7 @@ afs_vop_print(ap)
 /*
  * Advisory record locking support (fcntl() POSIX style)
  */
-int
+static int
 afs_vop_advlock(ap)
      struct vop_advlock_args	/* {
 				 * struct vnode *a_vp;
@@ -1601,15 +1310,63 @@ afs_vop_advlock(ap)
 				 * int  a_flags;
 				 * } */ *ap;
 {
-    int error;
+    int error, a_op;
     struct ucred cr = *osi_curcred();
 
+    a_op = ap->a_op;
+    if (a_op == F_UNLCK) {
+	/*
+	 * When a_fl->type is F_UNLCK, FreeBSD passes in an a_op of F_UNLCK.
+	 * This is (confusingly) different than how you actually release a lock
+	 * with fcntl(), which is done with an a_op of F_SETLK and an l_type of
+	 * F_UNLCK. Pretend we were given an a_op of F_SETLK in this case,
+	 * since this is what afs_lockctl expects.
+	 */
+	a_op = F_SETLK;
+    }
+
     AFS_GLOCK();
     error =
 	afs_lockctl(VTOAFS(ap->a_vp),
 		ap->a_fl,
-		ap->a_op, &cr,
+		a_op, &cr,
 		(int)(intptr_t)ap->a_id);	/* XXX: no longer unique! */
     AFS_GUNLOCK();
     return error;
 }
+
+struct vop_vector afs_vnodeops = {
+	.vop_default =		&default_vnodeops,
+	.vop_access =		afs_vop_access,
+	.vop_advlock =		afs_vop_advlock,
+	.vop_close =		afs_vop_close,
+	.vop_create =		afs_vop_create,
+	.vop_fsync =		afs_vop_fsync,
+	.vop_getattr =		afs_vop_getattr,
+	.vop_getpages =		afs_vop_getpages,
+	.vop_inactive =		afs_vop_inactive,
+	.vop_ioctl =		afs_vop_ioctl,
+	.vop_link =		afs_vop_link,
+	.vop_lookup =		afs_vop_lookup,
+	.vop_mkdir =		afs_vop_mkdir,
+	.vop_mknod =		afs_vop_mknod,
+	.vop_open =		afs_vop_open,
+	.vop_pathconf =		afs_vop_pathconf,
+	.vop_print =		afs_vop_print,
+	.vop_putpages =		afs_vop_putpages,
+	.vop_read =		afs_vop_read,
+	.vop_readdir =		afs_vop_readdir,
+	.vop_readlink =		afs_vop_readlink,
+	.vop_reclaim =		afs_vop_reclaim,
+	.vop_remove =		afs_vop_remove,
+	.vop_rename =		afs_vop_rename,
+	.vop_rmdir =		afs_vop_rmdir,
+	.vop_setattr =		afs_vop_setattr,
+	.vop_strategy =		afs_vop_strategy,
+	.vop_symlink =		afs_vop_symlink,
+	.vop_write =		afs_vop_write,
+};
+
+#if defined(AFS_FBSD_HAS_VOP_VECTOR_REGISTER)
+VFS_VOP_VECTOR_REGISTER(afs_vnodeops);
+#endif /* AFS_FBSD_HAS_VOP_VECTOR_REGISTER */
