--- src/aklog/aklog.c.orig	2021-01-14 21:08:41 UTC
+++ src/aklog/aklog.c
@@ -119,6 +119,7 @@
 #define DIRSTRING "/"		/* String form of above */
 #define VOLMARKER ':'		/* Character separating cellname from mntpt */
 #define VOLMARKERSTRING ":"	/* String form of above */
+#define AKIMP_LIFETIME_MAX 720  /* Max token lifetime for akimpersonate in hours (30 days) */
 
 typedef struct {
     char cell[BUFSIZ];
@@ -145,6 +146,7 @@ static int get_user_realm(krb5_context, char **);
 
 #define TRYAGAIN(x) (x == AKLOG_TRYAGAIN || \
 		     x == KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN || \
+		     x == KRB5_KT_NOTFOUND || \
 		     x == KRB5KRB_ERR_GENERIC)
 
 #if defined(HAVE_KRB5_PRINC_SIZE) || defined(krb5_princ_size)
@@ -167,6 +169,11 @@ static int get_user_realm(krb5_context, char **);
 #error "Must have either krb5_princ_size or krb5_principal_get_comp_string"
 #endif
 
+#if defined(HAVE_ENCODE_KRB5_TICKET)
+extern krb5_error_code encode_krb5_ticket (const krb5_ticket *rep,
+					   krb5_data **code);
+#endif
+
 #if !defined(HAVE_KRB5_ENCRYPT_TKT_PART) && defined(HAVE_ENCODE_KRB5_ENC_TKT_PART) && defined(HAVE_KRB5_C_ENCRYPT)
 extern krb5_error_code encode_krb5_enc_tkt_part (const krb5_enc_tkt_part *rep,
 						 krb5_data **code);
@@ -297,6 +304,8 @@ static char *client = NULL;     /* client principal fo
 static linked_list zsublist;	/* List of zephyr subscriptions */
 static linked_list hostlist;	/* List of host addresses */
 static linked_list authedcells;	/* List of cells already logged to */
+static int akimp_lifetime = 36000;  /* Lifetime for akimpersonate tokens. Default 10 hrs */ 
+static int akimplifetime_present = 0; /* Whether a lifetime was specified for akimpersonate */
 
 /* A com_error bodge. The idea here is that this routine lets us lookup
  * things in the system com_err, if the AFS one just tells us the error
@@ -859,7 +868,7 @@ rxkad_get_converted_token(krb5_context context, krb5_c
 static int
 rxkad_get_token(krb5_context context, struct afsconf_cell *cell, char *realm,
 		struct ktc_tokenUnion **token, char **authuser, int *foreign) {
-    krb5_creds *v5cred;
+    krb5_creds *v5cred = NULL;
     char *realmUsed = NULL;
     char *username = NULL;
     int status;
@@ -870,7 +879,7 @@ rxkad_get_token(krb5_context context, struct afsconf_c
 
     status = rxkad_get_ticket(context, realm, cell, &v5cred, &realmUsed);
     if (status)
-	return status;
+	goto out;
 
     if (do524)
 	status = rxkad_get_converted_token(context, v5cred, token, &username);
@@ -901,6 +910,8 @@ out:
 	free(realmUsed);
     if (username)
 	free(username);
+    if (v5cred)
+        krb5_free_creds(context, v5cred);
 
     return status;
 }
@@ -1095,7 +1106,7 @@ auth_to_cell(krb5_context context, const char *config,
 	    afs_dprintf("Setting tokens for cell %s\n", cellconf.name);
 	}
 
-#ifndef AFS_AIX51_ENV
+#if defined(AFS_AIX_ENV) && !defined(AFS_AIX51_ENV)
 	/* on AIX 4.1.4 with AFS 3.4a+ if a write is not done before
 	 * this routine, it will not add the token. It is not clear what
 	 * is going on here! So we will do the following operation.
@@ -1580,6 +1591,28 @@ main(int argc, char *argv[])
 	    }
 	    else
 		usage();
+        else if ((strcmp(argv[i], "-token-lifetime") == 0))
+	    if (++i < argc) {
+		status = util_GetInt32(argv[i], &akimp_lifetime);
+		if (status) {
+		    fprintf(stderr,
+			    "%s: invalid value specified for token-lifetime.\n",
+			    progname);
+		    exit(AKLOG_MISC);
+		}
+
+		if (akimp_lifetime < 0 || akimp_lifetime > AKIMP_LIFETIME_MAX) {
+		    fprintf(stderr,
+                            "%s: token-lifetime must be within 0 and %d hrs.\n",
+			    progname, AKIMP_LIFETIME_MAX);
+		    exit(AKLOG_MISC);
+		}
+
+		akimp_lifetime = akimp_lifetime * 60 * 60;
+		akimplifetime_present = TRUE;
+	    }
+	    else
+		usage();
 	else if ((strcmp(argv[i], "-principal") == 0))
 	    if (++i < argc) {
 		client = argv[i];
@@ -1691,6 +1724,13 @@ main(int argc, char *argv[])
 	}
     }
 
+    if (akimplifetime_present && !keytab) {
+	fprintf(stderr,
+	        "%s: -token-lifetime is valid only if -keytab is specified.\n",
+	        progname);
+	exit(AKLOG_MISC);
+    }
+
     /* If nothing was given, log to the local cell. */
     if ((cells.nelements + paths.nelements) == 0) {
 	struct passwd *pwd;
@@ -1824,14 +1864,12 @@ get_credv5_akimpersonate(krb5_context context,
 			 krb5_principal client_principal,
 			 time_t starttime,
 			 time_t endtime,
-			 int *allowed_enctypes,
 			 int *paddress,
 			 krb5_creds** out_creds /* out */ )
 {
-#if defined(USING_HEIMDAL) || (defined(HAVE_ENCODE_KRB5_ENC_TKT) && defined(HAVE_ENCODE_KRB5_TICKET) && defined(HAVE_KRB5_C_ENCRYPT))
+#if defined(USING_HEIMDAL) || (defined(HAVE_ENCODE_KRB5_ENC_TKT_PART) && defined(HAVE_ENCODE_KRB5_TICKET) && defined(HAVE_KRB5_C_ENCRYPT))
     krb5_error_code code;
     krb5_keytab kt = 0;
-    krb5_kt_cursor cursor[1];
     krb5_keytab_entry entry[1];
     krb5_ccache cc = 0;
     krb5_creds *creds = 0;
@@ -1852,14 +1890,11 @@ get_credv5_akimpersonate(krb5_context context,
     krb5_data * temp;
 #endif
     int i;
-    static int any_enctype[] = {0};
     *out_creds = 0;
     if (!(creds = malloc(sizeof *creds))) {
         code = ENOMEM;
         goto cleanup;
     }
-    if (!allowed_enctypes)
-        allowed_enctypes = any_enctype;
 
     cc = 0;
     enctype = 0; /* AKIMPERSONATE_IGNORE_ENCTYPE */
@@ -1878,54 +1913,17 @@ get_credv5_akimpersonate(krb5_context context,
         goto cleanup;
     }
 
-    if (service_principal) {
-        for (i = 0; (enctype = allowed_enctypes[i]) || !i; ++i) {
-	    code = krb5_kt_get_entry(context,
-				     kt,
-				     service_principal,
-				     kvno,
-				     enctype,
-				     entry);
-	    if (!code) {
-		if (allowed_enctypes[i])
-		    deref_keyblock_enctype(session_key) = allowed_enctypes[i];
-		break;
-	    }
-        }
-        if (code) {
-	    afs_com_err(progname, code,"while scanning keytab entries");
-	    goto cleanup;
-        }
-    } else {
-        krb5_keytab_entry new[1];
-        int best = -1;
-        memset(new, 0, sizeof *new);
-        if ((code = krb5_kt_start_seq_get(context, kt, cursor))) {
-            afs_com_err(progname, code, "while starting keytab scan");
-            goto cleanup;
-        }
-        while (!(code = krb5_kt_next_entry(context, kt, new, cursor))) {
-            for (i = 0;
-                    allowed_enctypes[i] && allowed_enctypes[i]
-		     != deref_entry_enctype(new); ++i)
-                ;
-            if ((!i || allowed_enctypes[i]) &&
-		(best < 0 || best > i)) {
-                krb5_free_keytab_entry_contents(context, entry);
-                *entry = *new;
-                memset(new, 0, sizeof *new);
-            } else krb5_free_keytab_entry_contents(context, new);
-        }
-        if ((i = krb5_kt_end_seq_get(context, kt, cursor))) {
-            afs_com_err(progname, i, "while ending keytab scan");
-            code = i;
-            goto cleanup;
-        }
-        if (best < 0) {
-            afs_com_err(progname, code, "while scanning keytab");
-            goto cleanup;
-        }
+    code = krb5_kt_get_entry(context,
+			     kt,
+			     service_principal,
+			     kvno,
+			     enctype,
+			     entry);
+    if (!code)
         deref_keyblock_enctype(session_key) = deref_entry_enctype(entry);
+    else {
+        afs_com_err(progname, code, "while scanning keytab entries");
+        goto cleanup;
     }
 
     /* Make Ticket */
@@ -1948,10 +1946,6 @@ get_credv5_akimpersonate(krb5_context context,
     enc_tkt_reply->authtime = starttime;
     enc_tkt_reply->starttime = temp_time;
     *enc_tkt_reply->starttime = starttime;
-#if 0
-    enc_tkt_reply->renew_till = temp_time + 1;
-    *enc_tkt_reply->renew_till = endtime;
-#endif
     enc_tkt_reply->endtime = endtime;
 #else
     if ((code = krb5_c_make_random_key(context,
@@ -2121,10 +2115,6 @@ cleanup:
     if (deref_enc_data(&ticket_reply->enc_part))
         free(deref_enc_data(&ticket_reply->enc_part));
     krb5_free_keytab_entry_contents(context, entry);
-    if (client_principal)
-        krb5_free_principal(context, client_principal);
-    if (service_principal)
-        krb5_free_principal(context, service_principal);
     if (cc)
         krb5_cc_close(context, cc);
     if (kt)
@@ -2144,7 +2134,6 @@ get_credv5(krb5_context context, char *name, char *ins
 {
     krb5_creds increds;
     krb5_error_code r;
-    static krb5_principal client_principal = 0;
 
     afs_dprintf("Getting tickets: %s%s%s@%s\n", name,
 	    (inst && inst[0]) ? "/" : "", inst ? inst : "", realm);
@@ -2156,50 +2145,63 @@ get_credv5(krb5_context context, char *name, char *ins
 				  name,
 				  (inst && strlen(inst)) ? inst : NULL,
 				  NULL))) {
-        return r;
+        goto out;
     }
 
 
     if (!_krb425_ccache) {
         r = krb5_cc_default(context, &_krb425_ccache);
 	if (r)
-	    return r;
+	    goto out;
     }
-    if (!client_principal) {
-	if (client) {
-	    r = krb5_parse_name(context, client,  &client_principal);
-	} else {
-	    r = krb5_cc_get_principal(context, _krb425_ccache, &client_principal);
-	}
-	if (r)
-	    return r;
+
+    if (client) {
+        r = krb5_parse_name(context, client,  &increds.client);
+    } else {
+        r = krb5_cc_get_principal(context, _krb425_ccache, &increds.client);
     }
 
-    increds.client = client_principal;
+    if (r)
+	goto out;
+
     increds.times.endtime = 0;
     if (do524)
 	/* Ask for DES since that is what V4 understands */
 	get_creds_enctype((&increds)) = ENCTYPE_DES_CBC_CRC;
 
     if (keytab) {
-	int allowed_enctypes[] = {
-	    ENCTYPE_DES_CBC_CRC, 0
-	};
+	afs_int32 start, end;
+	start = time(NULL);
 
+	if (akimp_lifetime == 0) {
+	    end = MAX_AFS_INT32;
+	} else {
+	    end = start + akimp_lifetime;
+	}
+
 	r = get_credv5_akimpersonate(context,
 				     keytab,
 				     increds.server,
 				     increds.client,
-				     300, ((~0U)>>1),
-				     allowed_enctypes,
+				     start, end,
 				     0 /* paddress */,
 				     creds /* out */);
     } else {
 	r = krb5_get_credentials(context, 0, _krb425_ccache, &increds, creds);
     }
-    return r;
-}
 
+ out:
+
+   if (increds.server) {
+       krb5_free_principal(context, increds.server);
+   }
+
+   if (increds.client) {
+       krb5_free_principal(context, increds.client);
+   }
+
+   return r;
+}
 
 static int
 get_user_realm(krb5_context context, char **realm)
