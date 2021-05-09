--- src/bozo/bnode.c.orig	2021-01-14 21:08:41 UTC
+++ src/bozo/bnode.c
@@ -706,10 +706,6 @@ SendNotifierData(int fd, struct bnode_proc *tp)
     bufp += strlen(bufp);
     (void)sprintf(bufp, "lastExit: %ld\n", afs_printable_int32_ld(tp->lastExit));
     bufp += strlen(bufp);
-#ifdef notdef
-    (void)sprintf(bufp, "lastSignal: %ld\n", afs_printable_int32_ld(tp->lastSignal));
-    bufp += strlen(bufp);
-#endif
     (void)sprintf(bufp, "flags: %ld\n", afs_printable_int32_ld(tp->flags));
     bufp += strlen(bufp);
     (void)sprintf(bufp, "END bnode_proc\n");
