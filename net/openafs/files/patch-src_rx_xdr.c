--- src/rx/xdr.c.orig	2018-07-08 22:21:12 UTC
+++ src/rx/xdr.c
@@ -338,7 +338,7 @@ xdr_enum(XDR * xdrs, enum_t * ep)
      * enums are treated as ints
      */
 
-    return (xdr_long(xdrs, (long *)ep));
+    return (xdr_int(xdrs, ep));
 
 }
 
