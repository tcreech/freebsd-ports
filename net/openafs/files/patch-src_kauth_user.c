--- src/kauth/user.c.orig	2021-01-14 21:08:41 UTC
+++ src/kauth/user.c
@@ -167,9 +167,6 @@ ka_UserAuthenticateGeneral(afs_int32 flags, char *name
     /* handle smoothly the case where no AFS system calls exists (yet) */
     (void)signal(SIGSYS, SIG_IGN);
 #endif
-#ifdef	AFS_DECOSF_ENV
-    (void)signal(SIGTRAP, SIG_IGN);
-#endif /* AFS_DECOSF_ENV */
     if (instance == 0)
 	instance = "";
     if (flags & KA_USERAUTH_ONLY_VERIFY) {
@@ -179,14 +176,9 @@ ka_UserAuthenticateGeneral(afs_int32 flags, char *name
 	    code = ka_VerifyUserToken(name, instance, realm, &key);
 	}
     } else {
-#ifdef AFS_DUX40_ENV
-	if (flags & KA_USERAUTH_DOSETPAG)
-	    afs_setpag();
-#else
 #if !defined(AFS_NT40_ENV)
 	if (flags & KA_USERAUTH_DOSETPAG)
 	    setpag();
-#endif
 #endif
 	if (flags & KA_USERAUTH_DOSETPAG2)
 	    dosetpag = 1;
