--- src/budb/db_dump.c.orig	2021-01-14 21:08:41 UTC
+++ src/budb/db_dump.c
@@ -771,26 +771,3 @@ writeDatabase(struct ubik_trans *ut, int fid)
     doneWriting(code);
     return (code);
 }
-
-
-#ifdef notdef
-
-afs_int32
-canWrite(int fid)
-{
-    afs_int32 in, out, except;
-    struct timeval tp;
-    afs_int32 code;
-
-    tp.tv_sec = 0;
-    tp.tv_usec = 0;
-
-    out = (1 << fid);
-    in = 0;
-    except = 0;
-
-    code = IOMGR_Select(32, &in, &out, &except, &tp);
-    return (code);
-}
-
-#endif /* notdef */
