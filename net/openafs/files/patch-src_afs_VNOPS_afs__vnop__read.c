--- src/afs/VNOPS/afs_vnop_read.c.orig	2016-11-30 20:06:42 UTC
+++ src/afs/VNOPS/afs_vnop_read.c
@@ -138,7 +138,9 @@ afs_MemRead(struct vcache *avc, struct u
 	trimlen = len;
 	afsio_trim(&tuio, trimlen);
 #endif
-	AFS_UIOMOVE(afs_zeros, trimlen, UIO_READ, tuiop, code);
+        // If this is disabled, don't do it. Some uiomove routines don't like
+        // uio_iovcnt != 1.
+	//AFS_UIOMOVE(afs_zeros, trimlen, UIO_READ, tuiop, code);
     }
 
     while (avc->f.m.Length > 0 && totalLength > 0) {
