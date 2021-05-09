--- src/auth/ktc.c.orig	2021-01-14 21:08:41 UTC
+++ src/auth/ktc.c
@@ -118,9 +118,7 @@ int afs_tf_dest_tkt(void);
 static void ktc_LocalCell(void);
 #endif /* AFS_KERBEROS_ENV */
 
-#ifdef AFS_DUX40_ENV
-#define PIOCTL afs_pioctl
-#elif defined(UKERNEL)
+#if defined(UKERNEL)
 #define PIOCTL(A,B,C,D) (errno = (call_syscall(AFSCALL_PIOCTL,A,B,C,D)), errno?-1:0)
 #else
 #define PIOCTL pioctl
@@ -299,6 +297,15 @@ SetToken(struct ktc_principal *aserver, struct ktc_tok
 #endif /* NO_AFS_CLIENT */
     if (code)
 	return KTC_PIOCTLFAIL;
+#if defined(AFS_LINUX26_ENV) && defined(SYS_keyctl)
+    else
+        /*
+         * If we're using keyring based PAGs and the SESSION_TO_PARENT keyctl
+         * is available, use it to copy the session keyring to the parent process
+         */
+        if (flags & AFS_SETTOK_SETPAG)
+            syscall(SYS_keyctl, KEYCTL_SESSION_TO_PARENT);
+#endif
     return 0;
 }
 
@@ -1111,11 +1118,6 @@ ktc_curpag(void)
   * For copying and distribution information, please see the file
   * <mit-copyright.h>.
   */
-
-#if 0
-#include <sys/file.h>
-#include <krb.h>
-#endif
 
 #define TOO_BIG -1
 #define TF_LCK_RETRY ((unsigned)2)	/* seconds to sleep before
