--- src/afs/LINUX/osi_vnodeops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_vnodeops.c
@@ -63,6 +63,12 @@
 #undef USE_FOP_ITERATE
 #endif
 
+/* Kernels from before 2.6.19 may not be able to return errors from
+ * d_revalidate. */
+#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
+# define ERRORS_FROM_D_REVALIDATE
+#endif
+
 int cachefs_noreadpage = 0;
 
 extern struct backing_dev_info *afs_backing_dev_info;
@@ -71,7 +77,7 @@ extern struct vcache *afs_globalVp;
 
 /* Handle interfacing with Linux's pagevec/lru facilities */
 
-#if defined(HAVE_LINUX_LRU_CACHE_ADD_FILE)
+#if defined(HAVE_LINUX_LRU_CACHE_ADD_FILE) || defined(HAVE_LINUX_LRU_CACHE_ADD)
 
 /*
  * Linux's lru_cache_add_file provides a simplified LRU interface without
@@ -90,7 +96,13 @@ afs_lru_cache_init(struct afs_lru_pages *alrupages)
 static inline void
 afs_lru_cache_add(struct afs_lru_pages *alrupages, struct page *page)
 {
+# if defined(HAVE_LINUX_LRU_CACHE_ADD)
+    lru_cache_add(page);
+# elif defined(HAVE_LINUX_LRU_CACHE_ADD_FILE)
     lru_cache_add_file(page);
+# else
+#  error need a kernel function to add a page to the kernel lru cache
+# endif
 }
 
 static inline void
@@ -171,7 +183,7 @@ afs_linux_VerifyVCache(struct vcache *avc, cred_t **re
 
     code = afs_CreateReq(&treq, credp);
     if (code == 0) {
-        code = afs_VerifyVCache2(avc, treq);
+	code = afs_VerifyVCache(avc, treq);
 	afs_DestroyReq(treq);
     }
 
@@ -413,7 +425,7 @@ afs_linux_readdir(struct file *fp, void *dirbuf, filld
 
     /* update the cache entry */
   tagain:
-    code = afs_convert_code(afs_VerifyVCache2(avc, treq));
+    code = afs_convert_code(afs_VerifyVCache(avc, treq));
     if (code)
 	goto out;
 
@@ -583,13 +595,11 @@ out1:
 extern int afs_xioctl(struct inode *ip, struct file *fp, unsigned int com,
 		      unsigned long arg);
 
-#if defined(HAVE_UNLOCKED_IOCTL) || defined(HAVE_COMPAT_IOCTL)
 static long afs_unlocked_xioctl(struct file *fp, unsigned int com,
                                unsigned long arg) {
     return afs_xioctl(FILE_INODE(fp), fp, com, arg);
 
 }
-#endif
 
 
 static int
@@ -885,14 +895,8 @@ struct file_operations afs_dir_fops = {
 #else
   .readdir =	afs_linux_readdir,
 #endif
-#ifdef HAVE_UNLOCKED_IOCTL
   .unlocked_ioctl = afs_unlocked_xioctl,
-#else
-  .ioctl =	afs_xioctl,
-#endif
-#ifdef HAVE_COMPAT_IOCTL
   .compat_ioctl = afs_unlocked_xioctl,
-#endif
   .open =	afs_linux_open,
   .release =	afs_linux_release,
   .llseek =	default_llseek,
@@ -920,14 +924,8 @@ struct file_operations afs_file_fops = {
   .read =	afs_linux_read,
   .write =	afs_linux_write,
 #endif
-#ifdef HAVE_UNLOCKED_IOCTL
   .unlocked_ioctl = afs_unlocked_xioctl,
-#else
-  .ioctl =	afs_xioctl,
-#endif
-#ifdef HAVE_COMPAT_IOCTL
   .compat_ioctl = afs_unlocked_xioctl,
-#endif
   .mmap =	afs_linux_mmap,
   .open =	afs_linux_open,
   .flush =	afs_linux_flush,
@@ -1126,8 +1124,13 @@ vattr2inode(struct inode *ip, struct vattr *vp)
  * Linux version of setattr call. What to change is in the iattr struct.
  * We need to set bits in both the Linux inode as well as the vcache.
  */
+#if defined(IOP_TAKES_USER_NAMESPACE)
 static int
+afs_notify_change(struct user_namespace *mnt_userns, struct dentry *dp, struct iattr *iattrp)
+#else
+static int
 afs_notify_change(struct dentry *dp, struct iattr *iattrp)
+#endif
 {
     struct vattr *vattr = NULL;
     cred_t *credp = crref();
@@ -1155,8 +1158,19 @@ out:
     return afs_convert_code(code);
 }
 
-#if defined(IOP_GETATTR_TAKES_PATH_STRUCT)
+#if defined(IOP_TAKES_USER_NAMESPACE)
 static int
+afs_linux_getattr(struct user_namespace *mnt_userns, const struct path *path, struct kstat *stat,
+		  u32 request_mask, unsigned int sync_mode)
+{
+	int err = afs_linux_revalidate(path->dentry);
+	if (!err) {
+		generic_fillattr(afs_ns, path->dentry->d_inode, stat);
+	}
+	return err;
+}
+#elif defined(IOP_GETATTR_TAKES_PATH_STRUCT)
+static int
 afs_linux_getattr(const struct path *path, struct kstat *stat, u32 request_mask, unsigned int sync_mode)
 {
 	int err = afs_linux_revalidate(path->dentry);
@@ -1214,6 +1228,8 @@ filter_enoent(int code)
     if (code == ENOENT && fatal_signal_pending(current)) {
         return EINTR;
     }
+#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,25)
+# error fatal_signal_pending not available, but it should be
 #endif
     return code;
 }
@@ -1299,6 +1315,7 @@ afs_linux_dentry_revalidate(struct dentry *dp, int fla
     struct afs_fakestat_state fakestate;
     int force_drop = 0;
     afs_uint32 parent_dv;
+    int code = 0;
 
 #ifdef LOOKUP_RCU
     /* We don't support RCU path walking */
@@ -1329,14 +1346,13 @@ afs_linux_dentry_revalidate(struct dentry *dp, int fla
 	if (vcp->mvstat == AFS_MVSTAT_MTPT) {
 	    if (vcp->mvid.target_root && (vcp->f.states & CMValid)) {
 		int tryEvalOnly = 0;
-		int code = 0;
 		struct vrequest *treq = NULL;
 
 		credp = crref();
 
 		code = afs_CreateReq(&treq, credp);
 		if (code) {
-		    goto bad_dentry;
+		    goto error;
 		}
 		if ((strcmp(dp->d_name.name, ".directory") == 0)) {
 		    tryEvalOnly = 1;
@@ -1346,7 +1362,10 @@ afs_linux_dentry_revalidate(struct dentry *dp, int fla
 		else
 		    code = afs_EvalFakeStat(&vcp, &fakestate, treq);
 		afs_DestroyReq(treq);
-		if ((tryEvalOnly && vcp->mvstat == AFS_MVSTAT_MTPT) || code) {
+		if (code != 0) {
+		    goto error;
+		}
+		if (tryEvalOnly && vcp->mvstat == AFS_MVSTAT_MTPT) {
 		    /* a mount point, not yet replaced by its directory */
 		    goto bad_dentry;
 		}
@@ -1355,20 +1374,6 @@ afs_linux_dentry_revalidate(struct dentry *dp, int fla
 	    osi_Assert(vcp->mvid.parent != NULL);
 	}
 
-#ifdef notdef
-	/* If the last looker changes, we should make sure the current
-	 * looker still has permission to examine this file.  This would
-	 * always require a crref() which would be "slow".
-	 */
-	if (vcp->last_looker != treq.uid) {
-	    if (!afs_AccessOK(vcp, (vType(vcp) == VREG) ? PRSFS_READ : PRSFS_LOOKUP, &treq, CHECK_MODE_BITS)) {
-		goto bad_dentry;
-	    }
-
-	    vcp->last_looker = treq.uid;
-	}
-#endif
-
 	parent = dget_parent(dp);
 	pvcp = VTOAFS(parent->d_inode);
 	parent_dv = parent_vcache_dv(parent->d_inode, credp);
@@ -1380,22 +1385,27 @@ afs_linux_dentry_revalidate(struct dentry *dp, int fla
 
 	if (parent_dv > dp->d_time || !(vcp->f.states & CStatd)) {
 	    struct vattr *vattr = NULL;
-	    int code;
-	    int lookup_good;
 
 	    if (credp == NULL) {
 		credp = crref();
 	    }
 	    code = afs_lookup(pvcp, (char *)dp->d_name.name, &tvc, credp);
             code = filter_enoent(code);
+	    if (code == ENOENT) {
+		/* ENOENT is not an error here. */
+		code = 0;
+		osi_Assert(tvc == NULL);
+	    }
 
 	    if (code) {
-		/* We couldn't perform the lookup, so we're not okay. */
-		lookup_good = 0;
+		/* We couldn't perform the lookup, so we don't know if the
+		 * dentry is valid or not. */
+		dput(parent);
+		goto error;
+	    }
 
-	    } else if (tvc == vcp) {
+	    if (tvc == vcp) {
 		/* We got back the same vcache, so we're good. */
-		lookup_good = 1;
 
 	    } else if (tvc == VTOAFS(dp->d_inode)) {
 		/* We got back the same vcache, so we're good. This is
@@ -1406,37 +1416,29 @@ afs_linux_dentry_revalidate(struct dentry *dp, int fla
 		 * versa, so the previous case will not succeed. But this is
 		 * still 'correct', so make sure not to mark the dentry as
 		 * invalid; it still points to the same thing! */
-		lookup_good = 1;
 
 	    } else {
-		/* We got back a different file, so we're definitely not
-		 * okay. */
-		lookup_good = 0;
-	    }
-
-	    if (!lookup_good) {
+		/*
+		 * We got back a different file, so we know this dentry is
+		 * _not_ okay. Force it to be unhashed, since the given name
+		 * doesn't point to this file anymore.
+		 */
 		dput(parent);
-		/* Force unhash; the name doesn't point to this file
-		 * anymore. */
 		force_drop = 1;
-		if (code && code != ENOENT) {
-		    /* ...except if we couldn't perform the actual lookup,
-		     * we don't know if the name points to this file or not. */
-		    force_drop = 0;
-		}
 		goto bad_dentry;
 	    }
 
 	    code = afs_CreateAttr(&vattr);
 	    if (code) {
 		dput(parent);
-		goto bad_dentry;
+		goto error;
 	    }
 
 	    if (afs_getattr(vcp, vattr, credp)) {
 		dput(parent);
 		afs_DestroyAttr(vattr);
-		goto bad_dentry;
+		code = EIO;
+		goto error;
 	    }
 
 	    vattr2inode(AFSTOV(vcp), vattr);
@@ -1468,10 +1470,12 @@ afs_linux_dentry_revalidate(struct dentry *dp, int fla
     }
 
   good_dentry:
+    code = 0;
     valid = 1;
     goto done;
 
   bad_dentry:
+    code = 0;
     valid = 0;
 #ifndef D_INVALIDATE_IS_VOID
     /* When (v3.18) d_invalidate was converted to void, it also started
@@ -1497,6 +1501,18 @@ afs_linux_dentry_revalidate(struct dentry *dp, int fla
     if (credp)
 	crfree(credp);
 
+#ifdef ERRORS_FROM_D_REVALIDATE
+    if (code != 0) {
+	/*
+	 * If code is nonzero, we don't know whether this dentry is valid or
+	 * not; we couldn't successfully perform the relevant lookup in order
+	 * to tell. So we must not return 'valid' (1) or 'not valid' (0); we
+	 * need to return an error (e.g. -EIO).
+	 */
+	return -code;
+    }
+#endif
+
 #ifndef D_INVALIDATE_IS_VOID
     if (!valid) {
 	/*
@@ -1513,6 +1529,17 @@ afs_linux_dentry_revalidate(struct dentry *dp, int fla
 #endif
     return valid;
 
+ error:
+    if (code <= 0) {
+	code = EIO;
+    }
+#ifdef ERRORS_FROM_D_REVALIDATE
+    valid = 0;
+    goto done;
+#else
+    /* We can't return an error, so default to saying the dentry is invalid. */
+    goto bad_dentry;
+#endif
 }
 
 static void
@@ -1611,17 +1638,25 @@ struct dentry_operations afs_dentry_operations = {
  *
  * name is in kernel space at this point.
  */
+
+#if defined(IOP_TAKES_USER_NAMESPACE)
 static int
-#if defined(IOP_CREATE_TAKES_BOOL)
+afs_linux_create(struct user_namespace *mnt_userns, struct inode *dip,
+		 struct dentry *dp, umode_t mode, bool excl)
+#elif defined(IOP_CREATE_TAKES_BOOL)
+static int
 afs_linux_create(struct inode *dip, struct dentry *dp, umode_t mode,
 		 bool excl)
 #elif defined(IOP_CREATE_TAKES_UMODE_T)
+static int
 afs_linux_create(struct inode *dip, struct dentry *dp, umode_t mode,
 		 struct nameidata *nd)
 #elif defined(IOP_CREATE_TAKES_NAMEIDATA)
+static int
 afs_linux_create(struct inode *dip, struct dentry *dp, int mode,
 		 struct nameidata *nd)
 #else
+static int
 afs_linux_create(struct inode *dip, struct dentry *dp, int mode)
 #endif
 {
@@ -1896,8 +1931,14 @@ afs_linux_unlink(struct inode *dip, struct dentry *dp)
 }
 
 
+#if defined(IOP_TAKES_USER_NAMESPACE)
 static int
+afs_linux_symlink(struct user_namespace *mnt_userns, struct inode *dip,
+		  struct dentry *dp, const char *target)
+#else
+static int
 afs_linux_symlink(struct inode *dip, struct dentry *dp, const char *target)
+#endif
 {
     int code;
     cred_t *credp = crref();
@@ -1925,10 +1966,15 @@ out:
     return afs_convert_code(code);
 }
 
+#if defined(IOP_TAKES_USER_NAMESPACE)
 static int
-#if defined(IOP_MKDIR_TAKES_UMODE_T)
+afs_linux_mkdir(struct user_namespace *mnt_userns, struct inode *dip,
+		struct dentry *dp, umode_t mode)
+#elif defined(IOP_MKDIR_TAKES_UMODE_T)
+static int
 afs_linux_mkdir(struct inode *dip, struct dentry *dp, umode_t mode)
 #else
+static int
 afs_linux_mkdir(struct inode *dip, struct dentry *dp, int mode)
 #endif
 {
@@ -2000,13 +2046,22 @@ afs_linux_rmdir(struct inode *dip, struct dentry *dp)
 }
 
 
+#if defined(IOP_TAKES_USER_NAMESPACE)
 static int
+afs_linux_rename(struct user_namespace *mnt_userns,
+		 struct inode *oldip, struct dentry *olddp,
+		 struct inode *newip, struct dentry *newdp,
+		 unsigned int flags)
+#elif defined(HAVE_LINUX_INODE_OPERATIONS_RENAME_TAKES_FLAGS)
+static int
 afs_linux_rename(struct inode *oldip, struct dentry *olddp,
-		 struct inode *newip, struct dentry *newdp
-#ifdef HAVE_LINUX_INODE_OPERATIONS_RENAME_TAKES_FLAGS
-		 , unsigned int flags
+		 struct inode *newip, struct dentry *newdp,
+		 unsigned int flags)
+#else
+static int
+afs_linux_rename(struct inode *oldip, struct dentry *olddp,
+		 struct inode *newip, struct dentry *newdp)
 #endif
-		)
 {
     int code;
     cred_t *credp = crref();
@@ -2014,7 +2069,8 @@ afs_linux_rename(struct inode *oldip, struct dentry *o
     const char *newname = newdp->d_name.name;
     struct dentry *rehash = NULL;
 
-#ifdef HAVE_LINUX_INODE_OPERATIONS_RENAME_TAKES_FLAGS
+#if defined(HAVE_LINUX_INODE_OPERATIONS_RENAME_TAKES_FLAGS) || \
+    defined(IOP_TAKES_USER_NAMESPACE)
     if (flags)
 	return -EINVAL;		/* no support for new flags yet */
 #endif
@@ -2365,6 +2421,9 @@ afs_linux_readpage_fastpath(struct file *fp, struct pa
     return 1;
 
 out:
+    if (cacheFp != NULL) {
+	filp_close(cacheFp, NULL);
+    }
     ReleaseWriteLock(&avc->lock);
     ReleaseReadLock(&tdc->lock);
     afs_PutDCache(tdc);
@@ -3036,12 +3095,18 @@ done:
 /* afs_linux_permission
  * Check access rights - returns error if can't check or permission denied.
  */
+
+#if defined(IOP_TAKES_USER_NAMESPACE)
 static int
-#if defined(IOP_PERMISSION_TAKES_FLAGS)
+afs_linux_permission(struct user_namespace *mnt_userns, struct inode *ip, int mode)
+#elif defined(IOP_PERMISSION_TAKES_FLAGS)
+static int
 afs_linux_permission(struct inode *ip, int mode, unsigned int flags)
 #elif defined(IOP_PERMISSION_TAKES_NAMEIDATA)
+static int
 afs_linux_permission(struct inode *ip, int mode, struct nameidata *nd)
 #else
+static int
 afs_linux_permission(struct inode *ip, int mode)
 #endif
 {
