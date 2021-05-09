--- src/cf/linux-kernel-sig.m4.orig	2021-01-14 21:08:41 UTC
+++ src/cf/linux-kernel-sig.m4
@@ -14,4 +14,18 @@ AC_CHECK_LINUX_OPERATION([inode_operations], [rename],
                          [struct inode *oinode, struct dentry *odentry,
                          struct inode *ninode, struct dentry *ndentry,
                          unsigned int flags])
-])
+dnl Linux 5.12 added the user_namespace parameter to the several
+dnl inode operations functions.
+dnl Perform a generic test using the inode_op create to test for this change.
+AC_CHECK_LINUX_OPERATION([inode_operations], [create], [user_namespace],
+                         [#include <linux/fs.h>],
+                         [int],
+                         [struct user_namespace *mnt_userns,
+                         struct inode *inode, struct dentry *dentry,
+                         umode_t umode, bool flag])
+dnl if HAVE_LINUX_INODE_OPERATIONS_CREATE_USER_NAMESPACE, create a more generic
+dnl define.
+AS_IF([test AS_VAR_GET([ac_cv_linux_operation_inode_operations_create_user_namespace]) = yes],
+      [AC_DEFINE([IOP_TAKES_USER_NAMESPACE], 1,
+                 [define if inodeops require struct user_namespace])])
+])
\ No newline at end of file
