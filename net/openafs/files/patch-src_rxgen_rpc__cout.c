--- src/rxgen/rpc_cout.c.orig	2021-01-14 21:08:41 UTC
+++ src/rxgen/rpc_cout.c
@@ -122,14 +122,7 @@ print_header(definition * def)
     f_print(fout, "bool_t\n");
     f_print(fout, "xdr_%s(XDR *xdrs, ", def->def_name);
     f_print(fout, "%s ", def->def_name);
-#if 0
-    if (def->def_kind != DEF_TYPEDEF
-	|| !isvectordef(def->def.ty.old_type, def->def.ty.rel)) {
-	f_print(fout, "*");
-    }
-#else
     f_print(fout, "*");
-#endif
     f_print(fout, "objp)\n");
     f_print(fout, "{\n");
 }
