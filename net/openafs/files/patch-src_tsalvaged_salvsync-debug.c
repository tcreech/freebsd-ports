--- src/tsalvaged/salvsync-debug.c.orig	2021-01-14 21:08:41 UTC
+++ src/tsalvaged/salvsync-debug.c
@@ -355,25 +355,6 @@ reason_code_to_string(afs_int32 reason)
     }
 }
 
-#if 0
-static char *
-program_type_to_string(afs_int32 type)
-{
-    switch ((ProgramType)type) {
-    case fileServer:
-	return "fileServer";
-    case volumeUtility:
-	return "volumeUtility";
-    case salvager:
-	return "salvager";
-    case salvageServer:
-	return "salvageServer";
-    default:
-	return "**UNKNOWN**";
-    }
-}
-#endif
-
 static char *
 state_code_to_string(afs_int32 state)
 {
