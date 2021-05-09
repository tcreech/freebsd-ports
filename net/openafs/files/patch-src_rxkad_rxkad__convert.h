--- src/rxkad/rxkad_convert.h.orig	2021-01-14 21:08:41 UTC
+++ src/rxkad/rxkad_convert.h
@@ -26,13 +26,6 @@ ktc_to_cblockptr(struct ktc_encryptionKey *key) {
     return (DES_cblock *)key;
 }
 
-#if 0
-static_inline unsigned char *
-cblockptr_to_cblock(DES_cblock *key) {
-    return (unsigned char *)key;
-}
-#endif
-
 static_inline DES_cblock *
 charptr_to_cblock(char *key) {
     return (DES_cblock *)key;
