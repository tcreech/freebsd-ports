--- src/afs/NBSD/osi_file.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/NBSD/osi_file.c
@@ -129,9 +129,6 @@ osi_UFSTruncate(struct osi_file *afile, afs_int32 asiz
 void
 osi_DisableAtimes(struct vnode *avp)
 {
-#if 0
-    VTOI(avp)->i_flag &= ~IN_ACCESS;
-#endif
 }
 
 
