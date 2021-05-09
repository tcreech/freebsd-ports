--- src/lwp/process.c.orig	2021-01-14 21:08:41 UTC
+++ src/lwp/process.c
@@ -18,7 +18,7 @@
 
 #include "lwp.h"
 
-#if defined(AFS_OSF_ENV) || defined(AFS_S390_LINUX20_ENV)
+#if defined(AFS_S390_LINUX20_ENV)
 extern int PRE_Block;		/* used in lwp.c and process.s */
 #else
 extern char PRE_Block;		/* used in lwp.c and process.s */
@@ -26,23 +26,27 @@ extern char PRE_Block;		/* used in lwp.c and process.s
 
 #if defined(USE_UCONTEXT) && defined(HAVE_UCONTEXT_H)
 
+# if defined(AFS_LINUX20_ENV) || defined(AFS_XBSD_ENV)
+#  define AFS_UCONTEXT_NOSTACK
+# endif
+
 afs_int32
 savecontext(void (*ep) (void), struct lwp_context *savearea, char *newsp)
 {
-#if defined(AFS_LINUX20_ENV)
+# ifdef AFS_UCONTEXT_NOSTACK
     /* getcontext does not export stack info */
     int stackvar;
-#endif
+# endif
 
     PRE_Block = 1;
 
     savearea->state = 0;
     getcontext(&savearea->ucontext);
-#if defined(AFS_LINUX20_ENV)
+# ifdef AFS_UCONTEXT_NOSTACK
     savearea->topstack = (char *)&stackvar;
-#else
+# else
     savearea->topstack = savearea->ucontext.uc_stack.ss_sp;
-#endif
+# endif
     switch (savearea->state) {
     case 0:
 	if (newsp) {
