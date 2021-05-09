--- src/WINNT/kfw/inc/krb4/krb.h.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/kfw/inc/krb4/krb.h
@@ -43,14 +43,6 @@ typedef unsigned short gid_t;
  * the realm.
  */
 
-#ifdef notdef
-/* this is server - only, does not belong here; */
-#define         KRBLOG          "/kerberos/kerberos.log"
-/* are these used anyplace? */
-#define         VX_KRB_HSTFILE  "/etc/krbhst"
-#define         PC_KRB_HSTFILE  "\\kerberos\\krbhst"
-#endif
-
 #ifdef PC
 
 #define         KRB_CONF        "krb.con"
