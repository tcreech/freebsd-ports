--- src/libadmin/test/vos.c.orig	2021-01-14 21:08:41 UTC
+++ src/libadmin/test/vos.c
@@ -135,14 +135,6 @@ GetAddressFromString(const char *addr_str)
     return ntohl(addr);
 }
 
-#if 0
-static void
-PrintMessage(vos_messageType_t type, char *message)
-{
-    printf("%s\n", message);
-}
-#endif
-
 int
 DoVosBackupVolumeCreate(struct cmd_syndesc *as, void *arock)
 {
