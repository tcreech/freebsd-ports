--- src/uss/uss_common.c.orig	2021-01-14 21:08:41 UTC
+++ src/uss/uss_common.c
@@ -218,14 +218,8 @@ uss_common_FieldCp(char *a_to, char *a_from, char a_se
 	     */
 	    *a_overflowP = 1;
 	    a_to--;
-#if 0
-	    printf("*** Skipping overflow char '%c'\n", *(a_from - 1));
-#endif /* 0 */
 	    while (*a_from != a_separator && *a_from != '\0'
 		   && *a_from != '\n') {
-#if 0
-		printf("*** Skipping overflow char '%c'\n", *a_from);
-#endif /* 0 */
 		a_from++;
 	    }			/*Skip over excess chars */
 	    break;
