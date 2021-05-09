--- src/bozo/bos.c.orig	2021-01-14 21:08:41 UTC
+++ src/bozo/bos.c
@@ -139,7 +139,7 @@ GetConn(struct cmd_syndesc *as, int aencrypt)
 	exit(1);
     }
 
-    if (scIndex == RX_SECIDX_NULL)
+    if (scIndex == RX_SECIDX_NULL && !(secFlags & AFSCONF_SECOPTS_NOAUTH))
 	fprintf(stderr, "bos: running unauthenticated\n");
 
     tconn =
