--- src/rxkad/fcrypt.c.orig	2021-01-14 21:08:41 UTC
+++ src/rxkad/fcrypt.c
@@ -117,13 +117,8 @@ fc_ecb_encrypt(void * clear, void * cipher,
 #define Byte3 3
 #endif
 
-#if 0
-    memcpy(&L, clear, sizeof(afs_int32));
-    memcpy(&R, clear + 1, sizeof(afs_int32));
-#else
     L = ntohl(*((afs_uint32 *)clear));
     R = ntohl(*((afs_uint32 *)clear + 1));
-#endif
 
     if (encrypt) {
 	INC_RXKAD_STATS(fc_encrypts[ENCRYPT]);
@@ -163,13 +158,8 @@ fc_ecb_encrypt(void * clear, void * cipher,
 	    L ^= P;
 	}
     }
-#if 0
-    memcpy(cipher, &L, sizeof(afs_int32));
-    memcpy(cipher + 1, &R, sizeof(afs_int32));
-#else
     *((afs_int32 *)cipher) = htonl(L);
     *((afs_int32 *)cipher + 1) = htonl(R);
-#endif
     return 0;
 }
 
