--- src/afs/LINUX/osi_compat.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_compat.h
@@ -314,9 +314,22 @@ zero_user_segment(struct page *pp, unsigned int from1,
 }
 #endif
 
-#ifndef HAVE_LINUX_KERNEL_SETSOCKOPT
+#if defined(HAVE_LINUX_IP_SOCK_SET)
+# include <net/ip.h>
+/* ip_sock_set_* introduced in linux 5.8 */
+static inline void
+afs_linux_sock_set_mtu_discover(struct socket *sockp, int pmtu)
+{
+    ip_sock_set_mtu_discover(sockp->sk, pmtu);
+}
+static inline void
+afs_linux_sock_set_recverr(struct socket *sockp)
+{
+    ip_sock_set_recverr(sockp->sk);
+}
+#else
+# if !defined(HAVE_LINUX_KERNEL_SETSOCKOPT)
 /* Available from 2.6.19 */
-
 static inline int
 kernel_setsockopt(struct socket *sockp, int level, int name, char *val,
 		  unsigned int len) {
@@ -329,20 +342,22 @@ kernel_setsockopt(struct socket *sockp, int level, int
 
     return ret;
 }
+# endif /* !HAVE_LINUX_KERNEL_SETSOCKOPT */
 
-static inline int
-kernel_getsockopt(struct socket *sockp, int level, int name, char *val,
-		  int *len) {
-    mm_segment_t old_fs = get_fs();
-    int ret;
-
-    set_fs(get_ds());
-    ret = sockp->ops->getsockopt(sockp, level, name, val, len);
-    set_fs(old_fs);
-
-    return ret;
+static inline void
+afs_linux_sock_set_mtu_discover(struct socket *sockp, int pmtu)
+{
+    kernel_setsockopt(sockp, SOL_IP, IP_MTU_DISCOVER, (char *)&pmtu,
+		      sizeof(pmtu));
 }
-#endif
+static inline void
+afs_linux_sock_set_recverr(struct socket *sockp)
+{
+    int recverr = 1;
+    kernel_setsockopt(sockp, SOL_IP, IP_RECVERR, (char *)&recverr,
+		      sizeof(recverr));
+}
+#endif /* !HAVE_LINUX_IP_SOCK_SET */
 
 #ifdef HAVE_TRY_TO_FREEZE
 static inline int
@@ -509,7 +524,9 @@ afs_inode_setattr(struct osi_file *afile, struct iattr
 
     int code = 0;
     struct inode *inode = OSIFILE_INODE(afile);
-#if !defined(HAVE_LINUX_INODE_SETATTR)
+#if defined(IOP_TAKES_USER_NAMESPACE)
+    code = inode->i_op->setattr(afs_ns, afile->filp->f_dentry, newattrs);
+#elif !defined(HAVE_LINUX_INODE_SETATTR)
     code = inode->i_op->setattr(afile->filp->f_dentry, newattrs);
 #elif defined(INODE_SETATTR_NOT_VOID)
     if (inode->i_op && inode->i_op->setattr)
@@ -719,6 +736,29 @@ afs_file_write(struct file *filp, char __user *buf, si
 # endif
 #else
     return filp->f_op->write(filp, buf, len, pos);
+#endif
+}
+
+static inline char*
+afs_d_path(struct dentry *dp, struct vfsmount *mnt, char *buf, int buflen)
+{
+#ifdef D_PATH_TAKES_STRUCT_PATH
+    afs_linux_path_t p = { .mnt = mnt, .dentry = dp };
+    return d_path(&p, buf, buflen);
+#else
+    return d_path(dp, mnt, buf, buflen);
+#endif
+}
+
+static inline int
+afs_setattr_prepare(struct dentry *dp, struct iattr *newattrs)
+{
+#if defined(IOP_TAKES_USER_NAMESPACE)
+    return setattr_prepare(afs_ns, dp, newattrs);
+#elif defined(HAVE_LINUX_SETATTR_PREPARE)
+    return setattr_prepare(dp, newattrs);
+#else
+    return inode_change_ok(dp->d_inode, newattrs);
 #endif
 }
 
