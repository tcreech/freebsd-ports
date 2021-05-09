--- src/afs/afs_fetchstore.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_fetchstore.c
@@ -270,7 +270,9 @@ afs_GenericStoreProc(struct storeOps *ops, void *rock,
     size = tdc->f.chunkBytes;
 
     tfile = afs_CFileOpen(&tdc->f.inode);
-    osi_Assert(tfile);
+    if (!tfile) {
+	return EIO;
+    }
 
     while ( size > 0 ) {
 	code = (*ops->prepare)(rock, size, &tlen);
@@ -415,19 +417,6 @@ rxfs_storeInit(struct vcache *avc, struct afs_conn *tc
 	    osi_Panic
             ("rxfs_storeInit: osi_AllocSmallSpace for iovecs returned NULL\n");
 	*ops = (struct storeOps *) &rxfs_storeMemOps;
-#ifdef notdef
-	/* do this at a higher level now -- it's a parameter */
-	/* for now, only do 'continue from close' code if file fits in one
-	 * chunk.  Could clearly do better: if only one modified chunk
-	 * then can still do this.  can do this on *last* modified chunk */
-	length = avc->f.m.Length - 1; /* byte position of last byte we'll store */
-	if (shouldWake) {
-	    if (AFS_CHUNK(length) != 0)
-		*shouldWake = 0;
-	    else
-		*shouldWake = 1;
-	}
-#endif /* notdef */
     }
 
     *rock = (void *)v;
