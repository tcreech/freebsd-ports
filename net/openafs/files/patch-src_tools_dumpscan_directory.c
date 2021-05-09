--- src/tools/dumpscan/directory.c.orig	2021-01-14 21:08:41 UTC
+++ src/tools/dumpscan/directory.c
@@ -74,19 +74,6 @@ static int namehash(char *name, int buckets, int seed)
   return tval ? hval < 0 ? buckets - tval : tval : 0;
 }
 
-#if 0
-static void fixup(char *name, int l)
-{
-  name += 16;
-  l -= 15;
-
-  while (l-- > 0) {
-    name[0] = name[4];
-    name++;
-  }
-}
-#endif
-
 afs_uint32 parse_directory(XFILE *X, dump_parser *p, afs_vnode *v,
                         afs_uint32 size, int toeof)
 {
