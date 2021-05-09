--- src/afs/afs_buffer.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_buffer.c
@@ -236,28 +236,24 @@ DRead(struct dcache *adc, int page, struct DirBuffer *
         /* The directory blob is empty, apparently. This is not a valid dir
          * blob, so throw an error. */
         code = EIO;
+	goto error;
     } else if (page * AFS_BUFFER_PAGESIZE >= adc->f.chunkBytes) {
         code = ENOENT; /* past the end */
+	goto error;
     }
-    if (code) {
-	tb->fid = NULLIDX;
-	afs_reset_inode(&tb->inode);
-	tb->lockers--;
-	ReleaseWriteLock(&tb->lock);
-	return code;
-    }
+
     tfile = afs_CFileOpen(&adc->f.inode);
-    osi_Assert(tfile);
+    if (!tfile) {
+	code = EIO;
+	goto error;
+    }
     code =
 	afs_CFileRead(tfile, tb->page * AFS_BUFFER_PAGESIZE, tb->data,
 		      AFS_BUFFER_PAGESIZE);
     afs_CFileClose(tfile);
     if (code < AFS_BUFFER_PAGESIZE) {
-	tb->fid = NULLIDX;
-	afs_reset_inode(&tb->inode);
-	tb->lockers--;
-	ReleaseWriteLock(&tb->lock);
-	return EIO;
+	code = EIO;
+	goto error;
     }
     /* Note that findslot sets the page field in the buffer equal to
      * what it is searching for. */
@@ -265,6 +261,13 @@ DRead(struct dcache *adc, int page, struct DirBuffer *
     entry->buffer = tb;
     entry->data = tb->data;
     return 0;
+
+ error:
+    tb->fid = NULLIDX;
+    afs_reset_inode(&tb->inode);
+    tb->lockers--;
+    ReleaseWriteLock(&tb->lock);
+    return code;
 }
 
 static void
@@ -381,7 +384,9 @@ afs_newslot(struct dcache *adc, afs_int32 apage, struc
     if (lp->dirty) {
 	/* see DFlush for rationale for not getting and locking the dcache */
         tfile = afs_CFileOpen(&lp->inode);
-        osi_Assert(tfile);
+	if (!tfile)
+	    return NULL;    /* Callers will flag as EIO */
+
 	afs_CFileWrite(tfile, lp->page * AFS_BUFFER_PAGESIZE, lp->data,
 		       AFS_BUFFER_PAGESIZE);
 	lp->dirty = 0;
