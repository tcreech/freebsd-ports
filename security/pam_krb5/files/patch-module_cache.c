--- module/cache.c.orig	2020-03-29 18:48:11 UTC
+++ module/cache.c
@@ -45,8 +45,14 @@ pamk5_get_krb5ccname(struct pam_args *args, const char
         name = getenv(key);
     if (name == NULL || *name == '\0')
         return NULL;
-    else
+    else {
+        /* FreeBSD 12 pulled in changes from OpenPAM which seem to include
+         * adding a bug to pam_getenv. In particular, it always includes the
+         * '=' character at the beginning of the result. Hack in a fix. */
+        if (name[0] == '=')
+            name++;
         return name;
+    }
 }
 
 
