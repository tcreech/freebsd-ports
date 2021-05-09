--- src/WINNT/netidmgr_plugin/afsfuncs.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/netidmgr_plugin/afsfuncs.c
@@ -369,36 +369,6 @@ afs_list_tokens_internal(void)
             continue;
         }
 
-#if 0
-        /* failed attempt at trying to figure out the principal name from
-           the token.  The ticket that is attached to the token is not
-           in a form that is useful at this point */
-        idname[0] = L'\0';
-        if(atoken.kvno == RXKAD_TKT_TYPE_KERBEROS_V5) {
-            krb5_context ctx = 0;
-            krb5_ccache cc = 0;
-            krb5_creds * k5c;
-            krb5_error_code code;
-            char * princ;
-
-            code = khm_krb5_initialize(&ctx, &cc);
-            if(code)
-                goto _no_krb5;
-
-            k5c = (krb5_creds *) atoken.ticket;
-
-            code = krb5_unparse_name(ctx, k5c->client, &princ);
-            if(code)
-                goto _no_krb5;
-
-            MultiByteToWideChar(CP_ACP, 0, princ, strlen(princ), idname, sizeof(idname)/sizeof(idname[0]));
-
-            krb5_free_unparsed_name(ctx, princ);
-_no_krb5:
-            ;
-        }
-#endif
-
         method = AFS_TOKEN_AUTO;
 
         afs_princ_to_string(&aclient, idname, sizeof(idname));
