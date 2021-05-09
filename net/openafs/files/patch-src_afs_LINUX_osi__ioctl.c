--- src/afs/LINUX/osi_ioctl.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_ioctl.c
@@ -25,10 +25,6 @@
 #include <asm/ia32_unistd.h>
 #endif
 
-#if defined(AFS_SPARC64_LINUX26_ENV) && defined(NEED_IOCTL32) && !defined(HAVE_COMPAT_IOCTL)
-#include <linux/ioctl32.h>
-#endif
-
 #include <linux/slab.h>
 #include <linux/init.h>
 #include <linux/sched.h>
@@ -37,9 +33,6 @@
 #include "osi_compat.h"
 
 extern struct proc_dir_entry *openafs_procfs;
-#if defined(NEED_IOCTL32) && !defined(HAVE_COMPAT_IOCTL)
-static int ioctl32_done;
-#endif
 
 extern asmlinkage long
 afs_syscall(long syscall, long parm1, long parm2, long parm3, long parm4);
@@ -50,21 +43,13 @@ afs_ioctl(struct inode *inode, struct file *file, unsi
 {
 
     struct afsprocdata sysargs;
-#ifdef NEED_IOCTL32
-    struct afsprocdata32 sysargs32;
-#endif
 
     if (cmd != VIOC_SYSCALL && cmd != VIOC_SYSCALL32) return -EINVAL;
 
 #ifdef NEED_IOCTL32
-# if defined(AFS_S390X_LINUX26_ENV)
-    if (test_thread_flag(TIF_31BIT))
-# elif defined(AFS_AMD64_LINUX20_ENV)
-    if (test_thread_flag(TIF_IA32))
-# else
-    if (test_thread_flag(TIF_32BIT))
-# endif /* AFS_S390X_LINUX26_ENV */
-    {
+    if (afs_in_compat_syscall()) {
+	struct afsprocdata32 sysargs32;
+
 	if (copy_from_user(&sysargs32, (void *)arg,
 			   sizeof(struct afsprocdata32)))
 	    return -EFAULT;
@@ -85,12 +70,11 @@ afs_ioctl(struct inode *inode, struct file *file, unsi
     }
 }
 
-#if defined(HAVE_UNLOCKED_IOCTL) || defined(HAVE_COMPAT_IOCTL)
 static long afs_unlocked_ioctl(struct file *file, unsigned int cmd,
                                unsigned long arg) {
     return afs_ioctl(FILE_INODE(file), file, cmd, arg);
 }
-#endif
+
 #if defined(HAVE_LINUX_STRUCT_PROC_OPS)
 static struct proc_ops afs_syscall_ops = {
     .proc_ioctl = afs_unlocked_ioctl,
@@ -100,16 +84,11 @@ static struct proc_ops afs_syscall_ops = {
 };
 #else
 static struct file_operations afs_syscall_ops = {
-# ifdef HAVE_UNLOCKED_IOCTL
     .unlocked_ioctl = afs_unlocked_ioctl,
-# else
-    .ioctl = afs_ioctl,
-# endif
-# ifdef HAVE_COMPAT_IOCTL
     .compat_ioctl = afs_unlocked_ioctl,
-# endif
 };
 #endif /* HAVE_LINUX_STRUCT_PROC_OPS */
+
 void
 osi_ioctl_init(void)
 {
@@ -121,18 +100,10 @@ osi_ioctl_init(void)
 	entry->owner = THIS_MODULE;
 #endif
 
-#if defined(NEED_IOCTL32) && !defined(HAVE_COMPAT_IOCTL)
-    if (register_ioctl32_conversion(VIOC_SYSCALL32, NULL) == 0) 
-	ioctl32_done = 1;
-#endif
 }
 
 void
 osi_ioctl_clean(void)
 {
     remove_proc_entry(PROC_SYSCALL_NAME, openafs_procfs);
-#if defined(NEED_IOCTL32) && !defined(HAVE_COMPAT_IOCTL)
-    if (ioctl32_done)
-	    unregister_ioctl32_conversion(VIOC_SYSCALL32);
-#endif
 }
