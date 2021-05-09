--- src/ubik/phys.c.orig	2021-01-14 21:08:41 UTC
+++ src/ubik/phys.c
@@ -46,6 +46,29 @@ static char pbuffer[1024];
 
 static int uphys_buf_flush(struct ubik_dbase *adbase, afs_int32 afid);
 
+#ifdef HAVE_PIO
+# define uphys_pread  pread
+# define uphys_pwrite pwrite
+#else /* HAVE_PIO */
+static_inline ssize_t
+uphys_pread(int fd, void *buf, size_t nbyte, off_t offset)
+{
+    if (lseek(fd, offset, 0) < 0) {
+	return -1;
+    }
+    return read(fd, buf, nbyte);
+}
+
+static_inline ssize_t
+uphys_pwrite(int fd, void *buf, size_t nbyte, off_t offset)
+{
+    if (lseek(fd, offset, 0) < 0) {
+	return -1;
+    }
+    return write(fd, buf, nbyte);
+}
+#endif /* !HAVE_PIO */
+
 /*!
  * \warning Beware, when using this function, of the header in front of most files.
  */
@@ -72,7 +95,6 @@ uphys_open(struct ubik_dbase *adbase, afs_int32 afid)
     /* scan file descr cache */
     for (tfd = fdcache, i = 0; i < MAXFDCACHE; i++, tfd++) {
 	if (afid == tfd->fileID && tfd->refCount == 0) {	/* don't use open fd */
-	    lseek(tfd->fd, 0, 0);	/* reset ptr just like open would have */
 	    tfd->refCount++;
 	    return tfd->fd;
 	}
@@ -187,12 +209,7 @@ uphys_read(struct ubik_dbase *adbase, afs_int32 afile,
     fd = uphys_open(adbase, afile);
     if (fd < 0)
 	return -1;
-    code = lseek(fd, apos + HDRSIZE, 0);
-    if (code < 0) {
-	uphys_close(fd);
-	return -1;
-    }
-    code = read(fd, abuffer, alength);
+    code = uphys_pread(fd, abuffer, alength, apos + HDRSIZE);
     uphys_close(fd);
     return code;
 }
@@ -208,12 +225,7 @@ uphys_write(struct ubik_dbase *adbase, afs_int32 afile
     fd = uphys_open(adbase, afile);
     if (fd < 0)
 	return -1;
-    code = lseek(fd, apos + HDRSIZE, 0);
-    if (code < 0) {
-	uphys_close(fd);
-	return -1;
-    }
-    length = write(fd, abuffer, alength);
+    length = uphys_pwrite(fd, abuffer, alength, apos + HDRSIZE);
     code = uphys_close(fd);
     if (code)
 	return -1;
@@ -266,7 +278,7 @@ uphys_getlabel(struct ubik_dbase *adbase, afs_int32 af
     fd = uphys_open(adbase, afile);
     if (fd < 0)
 	return UNOENT;
-    code = read(fd, &thdr, sizeof(thdr));
+    code = uphys_pread(fd, &thdr, sizeof(thdr), 0);
     if (code != sizeof(thdr)) {
 	uphys_close(fd);
 	return EIO;
@@ -297,7 +309,7 @@ uphys_setlabel(struct ubik_dbase *adbase, afs_int32 af
     thdr.version.counter = htonl(aversion->counter);
     thdr.magic = htonl(UBIK_MAGIC);
     thdr.size = htons(HDRSIZE);
-    code = write(fd, &thdr, sizeof(thdr));
+    code = uphys_pwrite(fd, &thdr, sizeof(thdr), 0);
     fsync(fd);			/* preserve over crash */
     uphys_close(fd);
     if (code != sizeof(thdr)) {
