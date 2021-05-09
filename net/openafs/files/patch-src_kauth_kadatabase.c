--- src/kauth/kadatabase.c.orig	2021-01-14 21:08:41 UTC
+++ src/kauth/kadatabase.c
@@ -933,17 +933,3 @@ name_instance_legal(char *name, char *instance)
 	dynamic_statistics.string_checks++;
     return code;
 }
-
-#if 0
-static int
-string_legal(char *str, char *map)
-{
-    int slen;
-
-    slen = strlen(str);
-    if (slen >= MAXKTCNAMELEN)
-	return 0;		/* with trailing null must fit in data base */
-    return (slen == strspn(str, map));	/* strspn returns length(str) if all chars in map */
-}
-#endif
-
