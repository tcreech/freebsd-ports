--- src/WINNT/kfw/inc/krb5/krb5/krb5.h.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/kfw/inc/krb5/krb5/krb5.h
@@ -859,9 +859,6 @@ krb5_error_code krb5_decrypt_data
 #define	KRB5_PADATA_TGS_REQ		KRB5_PADATA_AP_REQ
 #define KRB5_PADATA_ENC_TIMESTAMP	2
 #define	KRB5_PADATA_PW_SALT		3
-#if 0				/* Not used */
-#define KRB5_PADATA_ENC_ENCKEY		4  /* Key encrypted within itself */
-#endif
 #define KRB5_PADATA_ENC_UNIX_TIME	5  /* timestamp encrypted in key */
 #define KRB5_PADATA_ENC_SANDIA_SECURID	6  /* SecurId passcode */
 #define KRB5_PADATA_SESAME		7  /* Sesame project */
