--- src/tests/afscp.c.orig	2021-01-14 21:08:41 UTC
+++ src/tests/afscp.c
@@ -164,29 +164,7 @@ do_rx_Init(void)
 struct rx_securityClass *
 get_sc(char *cellname)
 {
-#if 0
-    char realm[REALM_SZ];
-    CREDENTIALS c;
-#endif
-
     return rxnull_NewClientSecurityObject();
-#if 0
-
-    ucstring(realm, cellname, REALM_SZ);
-
-    if (krb_get_cred("afs", "", realm, &c)) {
-	if (get_ad_tkt("afs", "", realm, DEFAULT_TKT_LIFE)) {
-	    return NULL;
-	} else {
-	    if (krb_get_cred("afs", "", realm, &c)) {
-		return NULL;
-	    }
-	}
-    }
-
-    return rxkad_NewClientSecurityObject(rxkad_clear, c.session, c.kvno,
-					 c.ticket_st.length, c.ticket_st.dat);
-#endif
 }
 
 #define scindex_NULL 0
