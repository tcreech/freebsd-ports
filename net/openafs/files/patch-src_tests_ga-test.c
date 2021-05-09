--- src/tests/ga-test.c.orig	2021-01-14 21:08:41 UTC
+++ src/tests/ga-test.c
@@ -130,9 +130,6 @@ test_simple_strings(void)
 			    "--strings=foo", "--strings=bar", NULL}},
 	{AARG_GNUSTYLE, 5, {"strings", "-s", "foo", "-s", "bar", NULL}},
 	{AARG_AFSSTYLE, 4, {"strings", "-string", "foo", "bar", NULL}}
-#if 0
-	{AARG_AFSSTYLE, 3, {"strings", "foo", "bar", NULL}}
-#endif
     };
 
     struct agetargs args[] = {
@@ -243,10 +240,6 @@ test_simple_flag(void)
 	{AARG_GNUSTYLE, 2, {"flag", "-g", NULL}, GA_SUCCESS},
 	{AARG_AFSSTYLE, 2, {"flag", "--flag"}, GA_FAILURE},
 	{AARG_AFSSTYLE, 2, {"flag", "-flag", NULL}, GA_SUCCESS},
-#if 0
-	/* XXX */
-	{AARG_AFSSTYLE, 2, {"flag", "yes", NULL}, GA_SUCCESS},
-#endif
 	{AARG_GNUSTYLE, 2, {"flag", "--no-flag", NULL}, GA_SUCCESS}
     };
 
