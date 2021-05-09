--- src/afs/LINUX/osi_machdep.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_machdep.h
@@ -76,6 +76,8 @@
 #include "h/cred.h"
 #endif
 
+#include "afs/sysincludes.h"
+
 #if !defined(HAVE_LINUX_TIME_T)
 typedef time64_t time_t;
 #endif
@@ -122,7 +124,6 @@ static inline time_t osi_Time(void) {
 #undef gop_lookupname_user
 #define gop_lookupname_user osi_lookupname
 
-#define osi_vnhold(V, N) do { VN_HOLD(AFSTOV(V)); } while (0)
 #define VN_HOLD(V) osi_Assert(igrab((V)) == (V))
 #define VN_RELE(V) iput((V))
 
@@ -156,6 +157,46 @@ static inline long copyinstr(char *from, char *to, int
     return 0;
 }
 #define copyout(F, T, C) (copy_to_user ((char*)(T), (char*)(F), (C)) > 0 ? EFAULT : 0)
+
+/*
+ * Test to see for 64/32bit compatibility mode
+ * Return non-zero if in a 64bit kernel and handing a 32bit syscall
+ */
+#if defined(AFS_LINUX_64BIT_KERNEL) && !defined(AFS_ALPHA_LINUX20_ENV) && !defined(AFS_IA64_LINUX20_ENV)
+static inline int
+afs_in_compat_syscall(void)
+{
+# if defined(HAVE_LINUX_IN_COMPAT_SYSCALL)
+    return in_compat_syscall();
+# elif defined(AFS_SPARC64_LINUX26_ENV)
+    return test_thread_flag(TIF_32BIT);
+# elif defined(AFS_SPARC64_LINUX24_ENV)
+    return (current->thread.flags & SPARC_FLAG_32BIT) != 0;
+# elif defined(AFS_SPARC64_LINUX20_ENV)
+    return (current->tss.flags & SPARC_FLAG_32BIT) != 0;
+# elif defined(AFS_AMD64_LINUX26_ENV)
+    return test_thread_flag(TIF_IA32);
+# elif defined(AFS_AMD64_LINUX20_ENV)
+    return (current->thread.flags & THREAD_IA32) != 0;
+# elif defined(AFS_PPC64_LINUX26_ENV)
+#  if defined(STRUCT_TASK_STRUCT_HAS_THREAD_INFO)
+    return (current->thread_info->flags & _TIF_32BIT) != 0;
+#  else
+    return (task_thread_info(current)->flags & _TIF_32BIT) != 0;
+#  endif
+# elif defined(AFS_PPC64_LINUX20_ENV)
+   return (current->thread.flags & PPC_FLAG_32BIT) != 0;
+# elif defined(AFS_S390X_LINUX26_ENV)
+   return test_thread_flag(TIF_31BIT);
+# elif defined(AFS_S390X_LINUX20_ENV)
+  return (current->thread.flags & S390_FLAG_31BIT) != 0;
+# elif defined(AFS_ARM64_LINUX26_ENV)
+  return is_compat_task();
+# else
+#  error afs_in_compat_syscall not done for this linux
+# endif
+}
+#endif /* AFS_LINUX_64BIT_KERNEL */
 
 /* kernel print statements */
 #define printf(args...) printk(args)
