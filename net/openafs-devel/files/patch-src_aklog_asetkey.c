--- src/aklog/asetkey.c.orig	2019-05-20 00:40:26 UTC
+++ src/aklog/asetkey.c
@@ -237,84 +237,7 @@ addKey(struct afsconf_dir *dir, int argc, char **argv)
     }
 }
 
-static struct afsconf_typedKey *
-random_key(char **argv, int type, int kvno, int subtype)
-{
-    struct afsconf_typedKey *typedKey;
-    krb5_context ctx;
-    krb5_keyblock keyblock;
-    struct rx_opaque key;
-    int code;
-
-    code = krb5_init_context(&ctx);
-    if (code) {
-        afs_com_err(argv[0], code, "while initializing krb5 ctx");
-        exit(1);
-    }
-
-    memset(&keyblock, 0, sizeof(keyblock));
-    code = krb5_c_make_random_key(ctx, subtype, &keyblock);
-    if (code) {
-        afs_com_err(argv[0], code, "while generating random key");
-        exit(1);
-    }
-
-    memset(&key, 0, sizeof(key));
-    key.len = keyblock.length;
-    key.val = keyblock.contents;
-
-    typedKey = afsconf_typedKey_new(type, kvno, subtype, &key);
-
-    krb5_free_keyblock_contents(ctx, &keyblock);
-    krb5_free_context(ctx);
-
-    return typedKey;
-}
-
 static void
-addRandomKey(struct afsconf_dir *dir, int argc, char **argv)
-{
-    struct afsconf_typedKey *typedKey;
-    int type;
-    int kvno;
-    int code;
-    int subtype;
-
-    /* Just pick a reasonable enctype */
-    const int RAND_ENCTYPE = ENCTYPE_AES128_CTS_HMAC_SHA1_96;
-
-    subtype = RAND_ENCTYPE;
-
-    switch (argc) {
-    case 5:
-        subtype = atoi(argv[4]);
-        /* fall through */
-    case 4:
-        type = stringToType(argv[2]);
-        kvno = atoi(argv[3]);
-
-        typedKey = random_key(argv, type, kvno, subtype);
-
-        code = afsconf_AddTypedKey(dir, typedKey, 1);
-        afsconf_typedKey_put(&typedKey);
-        if (code) {
-            afs_com_err(argv[0], code, "while adding random key");
-            exit(1);
-        }
-
-        printf("Added random key with type %d kvno %d subtype %d\n",
-               type, kvno, subtype);
-        break;
-
-    default:
-	fprintf(stderr, "%s add-random: usage is '%s add-random <type> <kvno>\n",
-                argv[0], argv[0]);
-	fprintf(stderr, "\tOR\n\t%s add-random <type> <kvno> <subtype>\n", argv[0]);
-        exit(1);
-    }
-}
-
-static void
 deleteKey(struct afsconf_dir *dir, int argc, char **argv)
 {
     int type;
@@ -434,8 +357,6 @@ main(int argc, char *argv[])
 	fprintf(stderr, "\tOR\n\t%s add <type> <kvno> <subtype> <keyfile> <princ>\n",
 	        argv[0]);
 	fprintf(stderr, "\t\tEx: %s add 0 \"80b6a7cd7a9dadb6\"\n", argv[0]);
-	fprintf(stderr, "\t%s add-random <type> <kvno>\n", argv[0]);
-	fprintf(stderr, "\t%s add-random <type> <kvno> <subtype>\n", argv[0]);
 	fprintf(stderr, "\t%s delete <kvno>\n", argv[0]);
 	fprintf(stderr, "\t%s delete <type> <kvno>\n", argv[0]);
 	fprintf(stderr, "\t%s delete <type> <kvno> <subtype>\n", argv[0]);
@@ -460,9 +381,6 @@ main(int argc, char *argv[])
     else if (strcmp(argv[1], "list") == 0) {
 	listKey(tdir, argc, argv);
 
-    }
-    else if (strcmp(argv[1], "add-random") == 0) {
-        addRandomKey(tdir, argc, argv);
     }
     else {
 	fprintf(stderr, "%s: unknown operation '%s', type '%s' for "
