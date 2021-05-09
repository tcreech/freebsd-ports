--- src/afs/afs_syscall.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_syscall.c
@@ -114,40 +114,9 @@ copyin_afs_ioctl(caddr_t cmarg, struct afs_ioctl *dst)
 #endif /* defined(AFS_SGI_ENV) && (_MIPS_SZLONG==64) */
 
 #if defined(AFS_LINUX_64BIT_KERNEL) && !defined(AFS_ALPHA_LINUX20_ENV) && !defined(AFS_IA64_LINUX20_ENV)
-    struct afs_ioctl32 dst32;
+    if (afs_in_compat_syscall()) {
+	struct afs_ioctl32 dst32;
 
-#ifdef AFS_SPARC64_LINUX26_ENV
-    if (test_thread_flag(TIF_32BIT))
-#elif defined(AFS_SPARC64_LINUX24_ENV)
-    if (current->thread.flags & SPARC_FLAG_32BIT)
-#elif defined(AFS_SPARC64_LINUX20_ENV)
-    if (current->tss.flags & SPARC_FLAG_32BIT)
-
-#elif defined(AFS_AMD64_LINUX26_ENV)
-    if (test_thread_flag(TIF_IA32))
-#elif defined(AFS_AMD64_LINUX20_ENV)
-    if (current->thread.flags & THREAD_IA32)
-
-#elif defined(AFS_PPC64_LINUX26_ENV)
-#if defined(STRUCT_TASK_STRUCT_HAS_THREAD_INFO)
-    if (current->thread_info->flags & _TIF_32BIT)
-#else
-    if (task_thread_info(current)->flags & _TIF_32BIT)
-#endif
-#elif defined(AFS_PPC64_LINUX20_ENV)
-    if (current->thread.flags & PPC_FLAG_32BIT)
-
-#elif defined(AFS_S390X_LINUX26_ENV)
-    if (test_thread_flag(TIF_31BIT))
-#elif defined(AFS_S390X_LINUX20_ENV)
-    if (current->thread.flags & S390_FLAG_31BIT)
-#elif defined(AFS_ARM64_LINUX26_ENV)
-    if (is_compat_task())
-
-#else
-#error pioctl32 not done for this linux
-#endif
-    {
 	AFS_COPYIN(cmarg, (caddr_t) & dst32, sizeof dst32, code);
 	if (!code)
 	    afs_ioctl32_to_afs_ioctl(&dst32, dst);
@@ -391,40 +360,9 @@ copyin_iparam(caddr_t cmarg, struct iparam *dst)
 #endif /* AFS_SUN5_64BIT_ENV */
 
 #if defined(AFS_LINUX_64BIT_KERNEL) && !defined(AFS_ALPHA_LINUX20_ENV) && !defined(AFS_IA64_LINUX20_ENV)
-    struct iparam32 dst32;
+    if (afs_in_compat_syscall()) {
+	struct iparam32 dst32;
 
-#ifdef AFS_SPARC64_LINUX26_ENV
-    if (test_thread_flag(TIF_32BIT))
-#elif defined(AFS_SPARC64_LINUX24_ENV)
-    if (current->thread.flags & SPARC_FLAG_32BIT)
-#elif defined(AFS_SPARC64_LINUX20_ENV)
-    if (current->tss.flags & SPARC_FLAG_32BIT)
-
-#elif defined(AFS_AMD64_LINUX26_ENV)
-    if (test_thread_flag(TIF_IA32))
-#elif defined(AFS_AMD64_LINUX20_ENV)
-    if (current->thread.flags & THREAD_IA32)
-
-#elif defined(AFS_PPC64_LINUX26_ENV)
-#if defined(STRUCT_TASK_STRUCT_HAS_THREAD_INFO)
-    if (current->thread_info->flags & _TIF_32BIT)
-#else
-    if (task_thread_info(current)->flags & _TIF_32BIT)
-#endif
-#elif defined(AFS_PPC64_LINUX20_ENV)
-    if (current->thread.flags & PPC_FLAG_32BIT)
-
-#elif defined(AFS_S390X_LINUX26_ENV)
-    if (test_thread_flag(TIF_31BIT))
-#elif defined(AFS_S390X_LINUX20_ENV)
-    if (current->thread.flags & S390_FLAG_31BIT)
-#elif defined(AFS_ARM64_LINUX26_ENV)
-    if (is_compat_task())
-
-#else
-#error iparam32 not done for this linux platform
-#endif
-    {
 	AFS_COPYIN(cmarg, (caddr_t) & dst32, sizeof dst32, code);
 	if (!code)
 	    iparam32_to_iparam(&dst32, dst);
