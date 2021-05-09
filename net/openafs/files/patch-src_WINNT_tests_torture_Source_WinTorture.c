--- src/WINNT/tests/torture/Source/WinTorture.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/tests/torture/Source/WinTorture.c
@@ -534,15 +534,7 @@ int main(int argc, char *argv[])
     {
 #endif /* HAVE_HESOID */
         strcpy(AfsLocker, Locker);
-#if 0
-        /* jaltman - this makes no sense.  why strip off the server name and leave
-         * an absolute path on the local disk?
-         */
-        if (!strnicmp(Locker, "\\\\afs\\", strlen("\\\\afs\\")))
-            strcpy(TargetDirectory, &Locker[strlen("\\\\afs\\")]);
-        else
-#endif
-            strcpy(TargetDirectory, Locker);
+        strcpy(TargetDirectory, Locker);
 #ifdef HAVE_HESOID
     }
 #endif /* HAVE_HESOID */
