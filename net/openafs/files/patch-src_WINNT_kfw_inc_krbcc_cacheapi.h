--- src/WINNT/kfw/inc/krbcc/cacheapi.h.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/kfw/inc/krbcc/cacheapi.h
@@ -104,21 +104,6 @@ typedef struct opaque_dll_control_block_type* apiCB;
 typedef struct opaque_ccache_pointer_type* ccache_p;
 typedef struct opaque_credential_iterator_type* ccache_cit;
 
-#if 0
-enum _cc_data_type {
-    type_ticket = 0,                /* 0 for ticket, second_ticket */
-    /* Ted's draft spec says these are to be
-       "as defined in the Kerberos V5 protocol"
-       all I can find are typdefs,
-       can't find an enumerated type or #define
-    */
-    type_address,           /* =  <"as defined in the Kerberos V5 protocol"> */
-    type_authdata,          /* = <"as defined in the Kerberos V5 protocol"> */
-    type_encryption,        /* = <"as defined in the Kerberos V5 protocol"> */
-    cc_data_type_max        /* for validation */
-};
-#endif
-
 typedef struct _cc_data
 {
     cc_uint32       type;		// should be one of _cc_data_type
