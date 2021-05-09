--- src/cf/linux-kernel-func.m4.orig	2021-01-14 21:08:41 UTC
+++ src/cf/linux-kernel-func.m4
@@ -42,8 +42,13 @@ AC_CHECK_LINUX_FUNC([d_make_root],
 AC_CHECK_LINUX_FUNC([do_sync_read],
                     [#include <linux/fs.h>],
                     [do_sync_read(NULL, NULL, 0, NULL);])
+dnl - fatal_signal_pending introduced in 2.6.25
+dnl - moved from linux/sched.h to linux/sched/signal.h in 4.11
 AC_CHECK_LINUX_FUNC([fatal_signal_pending],
-                    [#include <linux/sched.h>],
+                    [#include <linux/sched.h>
+                     #ifdef HAVE_LINUX_SCHED_SIGNAL_H
+                     # include <linux/sched/signal.h>
+                     #endif],
                     [fatal_signal_pending(NULL);])
 AC_CHECK_LINUX_FUNC([file_dentry],
                     [#include <linux/fs.h>],
@@ -86,6 +91,9 @@ AC_CHECK_LINUX_FUNC([ktime_get_real_ts64],
 AC_CHECK_LINUX_FUNC([locks_lock_file_wait],
                     [#include <linux/fs.h>],
                     [locks_lock_file_wait(NULL, NULL);])
+AC_CHECK_LINUX_FUNC([override_creds],
+                    [#include <linux/cred.h>],
+                    [override_creds(0);])
 AC_CHECK_LINUX_FUNC([page_follow_link],
                     [#include <linux/fs.h>],
                     [page_follow_link(0,0);])
@@ -147,9 +155,28 @@ AC_CHECK_LINUX_FUNC([inode_lock],
                     [inode_lock(NULL);])
 
 dnl lru_cache_add_file added to Linux 2.6.28.
+dnl                    removed in Linux 5.8
 AC_CHECK_LINUX_FUNC([lru_cache_add_file],
                     [#include <linux/swap.h>],
                     [lru_cache_add_file(NULL);])
+
+dnl Linux 4.6 introduced in_compat_syscall as replacement for is_compat_task
+dnl for certain platforms.
+AC_CHECK_LINUX_FUNC([in_compat_syscall],
+                    [#include <linux/compat.h>],
+                    [in_compat_syscall();])
+
+dnl lru_cache_add exported in Linux 5.8
+dnl    replaces lru_cache_add_file
+AC_CHECK_LINUX_FUNC([lru_cache_add],
+                    [#include <linux/swap.h>],
+                    [lru_cache_add(NULL);])
+
+dnl Linux 5.8 replaced kernel_setsockopt with helper functions
+dnl e.g. ip_sock_set_mtu_discover, ip_sock_set_recverr
+AC_CHECK_LINUX_FUNC([ip_sock_set],
+                    [#include <net/ip.h>],
+                    [ip_sock_set_mtu_discover(NULL, 0);])
 
 dnl Consequences - things which get set as a result of the
 dnl                above tests
