--- src/WINNT/afsd/krb.h.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/krb.h
@@ -157,69 +157,6 @@ typedef struct ktext KTEXT_ST;
 /* Error code returned by krb_mk_safe */
 #define		SAFE_PRIV_ERROR	-1	/* syscall error */
 
-#if 0
-/*
- * macros for byte swapping; also scratch space
- * u_quad  0-->7, 1-->6, 2-->5, 3-->4, 4-->3, 5-->2, 6-->1, 7-->0
- * u_long  0-->3, 1-->2, 2-->1, 3-->0
- * u_short 0-->1, 1-->0
- */
-
-#define     swap_u_16(x) {\
- unsigned long   _krb_swap_tmp[4];\
- swab(((char *) x) +0, ((char *)  _krb_swap_tmp) +14 ,2); \
- swab(((char *) x) +2, ((char *)  _krb_swap_tmp) +12 ,2); \
- swab(((char *) x) +4, ((char *)  _krb_swap_tmp) +10 ,2); \
- swab(((char *) x) +6, ((char *)  _krb_swap_tmp) +8  ,2); \
- swab(((char *) x) +8, ((char *)  _krb_swap_tmp) +6 ,2); \
- swab(((char *) x) +10,((char *)  _krb_swap_tmp) +4 ,2); \
- swab(((char *) x) +12,((char *)  _krb_swap_tmp) +2 ,2); \
- swab(((char *) x) +14,((char *)  _krb_swap_tmp) +0 ,2); \
- memcpy((char *)x, (char *)_krb_swap_tmp, 16);\
-                            }
-
-#define     swap_u_12(x) {\
- unsigned long   _krb_swap_tmp[4];\
- swab(( char *) x,     ((char *)  _krb_swap_tmp) +10 ,2); \
- swab(((char *) x) +2, ((char *)  _krb_swap_tmp) +8 ,2); \
- swab(((char *) x) +4, ((char *)  _krb_swap_tmp) +6 ,2); \
- swab(((char *) x) +6, ((char *)  _krb_swap_tmp) +4 ,2); \
- swab(((char *) x) +8, ((char *)  _krb_swap_tmp) +2 ,2); \
- swab(((char *) x) +10,((char *)  _krb_swap_tmp) +0 ,2); \
- memcpy((char *)x, (char *)_krb_swap_tmp, 12);\
-                            }
-
-#define     swap_C_Block(x) {\
- unsigned long   _krb_swap_tmp[4];\
- swab(( char *) x,    ((char *)  _krb_swap_tmp) +6 ,2); \
- swab(((char *) x) +2,((char *)  _krb_swap_tmp) +4 ,2); \
- swab(((char *) x) +4,((char *)  _krb_swap_tmp) +2 ,2); \
- swab(((char *) x) +6,((char *)  _krb_swap_tmp)    ,2); \
- memcpy((char *)x, (char *)_krb_swap_tmp, 8);\
-                            }
-#define     swap_u_quad(x) {\
- unsigned long   _krb_swap_tmp[4];\
- swab(( char *) &x,    ((char *)  _krb_swap_tmp) +6 ,2); \
- swab(((char *) &x) +2,((char *)  _krb_swap_tmp) +4 ,2); \
- swab(((char *) &x) +4,((char *)  _krb_swap_tmp) +2 ,2); \
- swab(((char *) &x) +6,((char *)  _krb_swap_tmp)    ,2); \
- memcpy((char *)&x, (char *)_krb_swap_tmp, 8);\
-                            }
-
-#define     swap_u_long(x) {\
- unsigned long   _krb_swap_tmp[4];\
- swab((char *)  &x,    ((char *)  _krb_swap_tmp) +2 ,2); \
- swab(((char *) &x) +2,((char *)  _krb_swap_tmp),2); \
- x = _krb_swap_tmp[0];   \
-                           }
-
-#define     swap_u_short(x) {\
- unsigned short	_krb_swap_sh_tmp; \
- swab((char *)  &x,    ( &_krb_swap_sh_tmp) ,2); \
- x = (unsigned short) _krb_swap_sh_tmp; \
-                            }
-#endif /* 0 */
-
 #include "krb_prot.h"
 
 static send_to_kdc(
