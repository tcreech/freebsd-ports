--- src/rxkad/bg-fcrypt.c.orig	2021-01-14 21:08:41 UTC
+++ src/rxkad/bg-fcrypt.c
@@ -109,17 +109,6 @@
 
 #define EFF_NTOHL(x) ntohl(x)
 
-#if 0
-#if defined(__GNUC__) && (defined(i386) || defined(__i386__))
-static inline afs_uint32
-bswap(afs_uint32 x)
-{
-  asm("bswap %0": "=r"(x):"0"(x));
-    return x;
-}
-#endif
-#endif
-
 /*
  * Sboxes for Feistel network derived from
  * /afs/transarc.com/public/afsps/afs.rel31b.export-src/rxkad/sboxes.h
@@ -469,15 +458,8 @@ fc_cbc_dec(const afs_uint32 * in, afs_uint32 * out, af
 	xor0 = in[0] ^ b8[0];
 	xor1 = in[1] ^ b8[1];
 
-#if 0
-	if (length >= 8)
-	    memcpy(out, b8, 8);
-	else
-	    memcpy(out, b8, length);	/* Don't write to much when length < 8 */
-#else
 	/* If length < 8 we write to much, this is not always ok */
 	memcpy(out, b8, 8);
-#endif
 	out += 2;
 	in += 2;
     }
