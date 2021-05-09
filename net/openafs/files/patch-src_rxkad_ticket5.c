--- src/rxkad/ticket5.c.orig	2021-01-14 21:08:41 UTC
+++ src/rxkad/ticket5.c
@@ -272,12 +272,11 @@ tkt_DecodeTicket5(char *ticket, afs_int32 ticket_len,
 	    krb5_free_context(context);
 	    goto unknown_key;
 	}
-	code = krb5_enctype_keybits(context,  t5.enc_part.etype, &keysize);
+	code = krb5_enctype_keysize(context,  t5.enc_part.etype, &keysize);
 	if (code != 0) {
 	    krb5_free_context(context);
 	    goto unknown_key;
 	}
-	keysize = keysize / 8;
 	allocsiz = keysize;
 	keybuf = rxi_Alloc(allocsiz);
 	/* this is not quite a hole for afsconf_GetKeyByTypes. A wrapper
@@ -394,13 +393,8 @@ tkt_DecodeTicket5(char *ticket, afs_int32 ticket_len,
 	*start = decr_part.authtime;
 	if (decr_part.starttime)
 	    *start = *decr_part.starttime;
-#if 0
-	if (*start - now > CLOCK_SKEW || decr_part.flags.invalid)
-	    goto no_auth;
-#else
 	if (decr_part.flags.invalid)
 	    goto no_auth;
-#endif
 	if (now > decr_part.endtime)
 	    goto tkt_expired;
 	*end = decr_part.endtime;
