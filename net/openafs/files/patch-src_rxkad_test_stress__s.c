--- src/rxkad/test/stress_s.c.orig	2021-01-14 21:08:41 UTC
+++ src/rxkad/test/stress_s.c
@@ -277,13 +277,7 @@ SRXKST_Copious(struct rx_call *call, u_long inlen, u_l
 	code = RXKST_BADINPUTSUM;
 	goto done;
     }
-#define BIG_PRIME 1257056893	/* 0x4AED2A7d */
-#if 0
-#define NextByte() ((b>24 ? ((seed = seed*BIG_PRIME + BIG_PRIME),b=0) : 0), \
-		    (b +=8), ((seed >> (b-8))&0xff))
-#else
 #define NextByte() (b+=3)
-#endif
     b = 32;
 
     mysum = 0;
