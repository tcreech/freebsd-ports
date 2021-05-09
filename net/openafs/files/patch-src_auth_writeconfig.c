--- src/auth/writeconfig.c.orig	2021-01-14 21:08:41 UTC
+++ src/auth/writeconfig.c
@@ -123,6 +123,7 @@ afsconf_SetExtendedCellInfo(struct afsconf_dir *adir,
     i = (int)strlen(acellInfo->name);
     code = write(fd, acellInfo->name, i);
     if (code != i) {
+	close(fd);
 	UNLOCK_GLOBAL_MUTEX;
 	return AFSCONF_FAILURE;
     }
