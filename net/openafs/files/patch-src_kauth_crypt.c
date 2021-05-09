--- src/kauth/crypt.c.orig	2021-01-14 21:08:41 UTC
+++ src/kauth/crypt.c
@@ -908,62 +908,6 @@ init_perm(C_block perm[64 / CHUNKBITS][1 << CHUNKBITS]
     }
 }
 
-/*
- * "setkey" routine (for backwards compatibility)
- */
-#if 0				/* static and doesn't appear to be referenced */
-STATIC int
-setkey(key)
-     const char *key;
-{
-    int i, j, k;
-    C_block keyblock;
-
-    for (i = 0; i < 8; i++) {
-	k = 0;
-	for (j = 0; j < 8; j++) {
-	    k <<= 1;
-	    k |= (unsigned char)*key++;
-	}
-	keyblock.b[i] = k;
-    }
-    return (des_setkey((char *)keyblock.b));
-}
-#endif
-
-#if 0
-/*
- * "encrypt" routine (for backwards compatibility)
- */
-int
-encrypt(block, flag)
-     char *block;
-     int flag;
-{
-    int i, j, k;
-    C_block cblock;
-
-    for (i = 0; i < 8; i++) {
-	k = 0;
-	for (j = 0; j < 8; j++) {
-	    k <<= 1;
-	    k |= (unsigned char)*block++;
-	}
-	cblock.b[i] = k;
-    }
-    if (des_cipher((char *)&cblock, (char *)&cblock, 0L, (flag ? -1 : 1)))
-	return (1);
-    for (i = 7; i >= 0; i--) {
-	k = cblock.b[i];
-	for (j = 7; j >= 0; j--) {
-	    *--block = k & 01;
-	    k >>= 1;
-	}
-    }
-    return (0);
-}
-#endif
-
 #ifdef CRYPT_DEBUG
 STATIC
 prtab(char *s, unsigned char *t, int num_rows)
