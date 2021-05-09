--- src/auth/ktc_nt.c.orig	2021-01-14 21:08:41 UTC
+++ src/auth/ktc_nt.c
@@ -650,12 +650,8 @@ ktc_GetTokenEx(char *cellName, struct ktc_setTokenData
 	}
     }
 
-#if 0
-    code = pioctl(0, VIOC_GETTOK2, &iob, 0);
-#else
-    code = -1;   /* not yet implemented */
-	errno = EINVAL;
-#endif
+    code = -1;   /* VIOC_GETTOK2 not yet implemented */
+    errno = EINVAL;
 
     ReleaseMutex(ktcMutex);
     CloseHandle(ktcMutex);
@@ -1084,12 +1080,8 @@ ktc_ListTokensEx(int prevIndex, int *newIndex, char **
 	iob.out = tbuffer;
 	iob.out_size = sizeof(tbuffer);
 
-#if 0
-	code = pioctl(0, VIOC_GETTOK2, &iob, 0);
-#else
-    code = -1;      /* not yet implemented */
+	code = -1;      /* VIOC_GETTOK2 not yet implemented */
 	errno = EINVAL;
-#endif
 
 	/* Can't use new pioctl, so must use old one */
 	if (code == -1 && errno == EINVAL) {
