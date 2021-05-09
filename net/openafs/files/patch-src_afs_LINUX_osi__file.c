--- src/afs/LINUX/osi_file.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_file.c
@@ -59,6 +59,9 @@ afs_linux_raw_open(afs_dcache_id_t *ainode)
     struct inode *tip = NULL;
     struct dentry *dp = NULL;
     struct file* filp;
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+    const struct cred *cur_cred;
+#endif
 
     dp = afs_get_dentry_from_fh(afs_cacheSBp, ainode, cache_fh_len, cache_fh_type,
 		afs_fh_acceptable);
@@ -67,6 +70,9 @@ afs_linux_raw_open(afs_dcache_id_t *ainode)
     tip = dp->d_inode;
     tip->i_flags |= S_NOATIME;	/* Disable updating access times. */
 
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+    cur_cred = override_creds(cache_creds);
+#endif
 #if defined(STRUCT_TASK_STRUCT_HAS_CRED)
     /* Use stashed credentials - prevent selinux/apparmor problems  */
     filp = afs_dentry_open(dp, afs_cacheMnt, O_RDWR, cache_creds);
@@ -75,6 +81,9 @@ afs_linux_raw_open(afs_dcache_id_t *ainode)
 #else
     filp = dentry_open(dget(dp), mntget(afs_cacheMnt), O_RDWR);
 #endif
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+    revert_creds(cur_cred);
+#endif
     if (IS_ERR(filp)) {
 	afs_warn("afs: Cannot open cache file (code %d). Trying to continue, "
                  "but AFS accesses may return errors or panic the system\n",
@@ -169,10 +178,20 @@ afs_osi_Stat(struct osi_file *afile, struct osi_stat *
 int
 osi_UFSClose(struct osi_file *afile)
 {
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+    const struct cred *cur_cred;
+#endif
+
     AFS_STATCNT(osi_Close);
     if (afile) {
 	if (OSIFILE_INODE(afile)) {
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+	    cur_cred = override_creds(cache_creds);
+#endif
 	    filp_close(afile->filp, NULL);
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+	    revert_creds(cur_cred);
+#endif
 	}
     }
     kfree(afile);
@@ -186,15 +205,21 @@ osi_UFSTruncate(struct osi_file *afile, afs_int32 asiz
     struct osi_stat tstat;
     struct iattr newattrs;
     struct inode *inode = OSIFILE_INODE(afile);
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+    const struct cred *cur_cred;
+#endif
     AFS_STATCNT(osi_Truncate);
 
     /* This routine only shrinks files, and most systems
      * have very slow truncates, even when the file is already
      * small enough.  Check now and save some time.
      */
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+    cur_cred = override_creds(cache_creds);
+#endif
     code = afs_osi_Stat(afile, &tstat);
     if (code || tstat.size <= asize)
-	return code;
+	goto done;
     AFS_GUNLOCK();
     afs_linux_lock_inode(inode);
 #ifdef STRUCT_INODE_HAS_I_ALLOC_SEM
@@ -205,11 +230,7 @@ osi_UFSTruncate(struct osi_file *afile, afs_int32 asiz
     AFS_CURRENT_TIME(&newattrs.ia_ctime);
 
     /* avoid notify_change() since it wants to update dentry->d_parent */
-#ifdef HAVE_LINUX_SETATTR_PREPARE
-    code = setattr_prepare(file_dentry(afile->filp), &newattrs);
-#else
-    code = inode_change_ok(inode, &newattrs);
-#endif
+    code = afs_setattr_prepare(file_dentry(afile->filp), &newattrs);
     if (!code)
 	code = afs_inode_setattr(afile, &newattrs);
     if (!code)
@@ -220,6 +241,10 @@ osi_UFSTruncate(struct osi_file *afile, afs_int32 asiz
 #endif
     afs_linux_unlock_inode(inode);
     AFS_GLOCK();
+ done:
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+    revert_creds(cur_cred);
+#endif
     return code;
 }
 
@@ -380,6 +405,9 @@ osi_rdwr(struct osi_file *osifile, struct uio *uiop, i
     size_t count;
     unsigned long savelim;
     loff_t pos;
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+    const struct cred *cur_cred = override_creds(cache_creds);
+#endif
 
     savelim = current->TASK_STRUCT_RLIM[RLIMIT_FSIZE].rlim_cur;
     current->TASK_STRUCT_RLIM[RLIMIT_FSIZE].rlim_cur = RLIM_INFINITY;
@@ -435,6 +463,9 @@ osi_rdwr(struct osi_file *osifile, struct uio *uiop, i
 #endif /* AFS_FILE_NEEDS_SET_FS */
 
     current->TASK_STRUCT_RLIM[RLIMIT_FSIZE].rlim_cur = savelim;
+#if defined(HAVE_LINUX_OVERRIDE_CREDS)
+    revert_creds(cur_cred);
+#endif
 
     return code;
 }
