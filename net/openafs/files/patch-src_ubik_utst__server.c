--- src/ubik/utst_server.c.orig	2021-01-14 21:08:41 UTC
+++ src/ubik/utst_server.c
@@ -318,9 +318,6 @@ main(int argc, char **argv)
     }
 
     sc[0] = rxnull_NewServerSecurityObject();
-#if 0
-    sc[1] = rxvab_NewServerSecurityObject("applexx", 0);
-#endif
     tservice = rx_NewService(0, USER_SERVICE_ID, "Sample", sc, 1 /*2 */ ,
 			     SAMPLE_ExecuteRequest);
     if (tservice == (struct rx_service *)0) {
