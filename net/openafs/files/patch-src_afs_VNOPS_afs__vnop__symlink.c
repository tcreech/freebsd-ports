--- src/afs/VNOPS/afs_vnop_symlink.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/VNOPS/afs_vnop_symlink.c
@@ -43,6 +43,7 @@ afs_DisconCreateSymlink(struct vcache *avc, char *anam
     struct dcache *tdc;
     struct osi_file *tfile;
     afs_size_t offset, len;
+    int code = 0;
 
     tdc = afs_GetDCache(avc, 0, areq, &offset, &len, 0);
     if (!tdc) {
@@ -54,14 +55,18 @@ afs_DisconCreateSymlink(struct vcache *avc, char *anam
     avc->f.m.Length = len;
 
     ObtainWriteLock(&tdc->lock, 720);
+    tfile = afs_CFileOpen(&tdc->f.inode);
+    if (!tfile) {
+	code = EIO;
+	goto done;
+    }
     afs_AdjustSize(tdc, len);
     tdc->validPos = len;
-    tfile = afs_CFileOpen(&tdc->f.inode);
-    osi_Assert(tfile);
     afs_CFileWrite(tfile, 0, aname, len);
     afs_CFileClose(tfile);
+ done:
     ReleaseWriteLock(&tdc->lock);
-    return 0;
+    return code;
 }
 
 /* don't set CDirty in here because RPC is called synchronously */
