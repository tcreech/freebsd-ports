--- src/WINNT/kfw/inc/loadfuncs/loadfuncs-krb5.h.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/kfw/inc/loadfuncs/loadfuncs-krb5.h
@@ -107,28 +107,6 @@ TYPEDEF_FUNC(
     (krb5_context, krb5_ap_rep * )
     );
 
-/* Removed around the time of krb5_rc_* change... */
-#if 0
-TYPEDEF_FUNC(
-    void,
-    KRB5_CALLCONV,
-    krb5_free_safe,
-    (krb5_context, krb5_safe * )
-    );
-TYPEDEF_FUNC(
-    void,
-    KRB5_CALLCONV,
-    krb5_free_priv,
-    (krb5_context, krb5_priv * )
-    );
-TYPEDEF_FUNC(
-    void,
-    KRB5_CALLCONV,
-    krb5_free_priv_enc_part,
-    (krb5_context, krb5_priv_enc_part * )
-    );
-#endif
-
 TYPEDEF_FUNC(
     void,
     KRB5_CALLCONV,
