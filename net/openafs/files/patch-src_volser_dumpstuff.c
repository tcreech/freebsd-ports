--- src/volser/dumpstuff.c.orig	2021-01-14 21:08:41 UTC
+++ src/volser/dumpstuff.c
@@ -704,10 +704,8 @@ static int
 DumpFile(struct iod *iodp, int vnode, FdHandle_t * handleP)
 {
     int code = 0, error = 0;
-    afs_int32 pad = 0;
-    afs_foff_t offset = 0;
     afs_sfsize_t nbytes, howBig;
-    ssize_t n;
+    ssize_t n = 0;
     size_t howMany;
     afs_foff_t howFar = 0;
     byte *p;
@@ -776,60 +774,36 @@ DumpFile(struct iod *iodp, int vnode, FdHandle_t * han
 	return VOLSERDUMPERROR;
     }
 
-    for (nbytes = howBig; (nbytes && !error); nbytes -= howMany) {
+    for (nbytes = howBig; (nbytes && !error); ) {
 	if (nbytes < howMany)
 	    howMany = nbytes;
 
 	/* Read the data */
 	n = FDH_PREAD(handleP, p, howMany, howFar);
-	howFar += n;
+	if (n < 0) {
+	    Log("1 Volser: DumpFile: Error reading inode %s for vnode %d: %s\n",
+		PrintInode(stmp, handleP->fd_ih->ih_ino), vnode,
+		afs_error_message(errno));
+	    error = VOLSERDUMPERROR;
 
-	/* If read any good data and we null padded previously, log the
-	 * amount that we had null padded.
-	 */
-	if ((n > 0) && pad) {
-	    Log("1 Volser: DumpFile: Null padding file %d bytes at offset %lld\n", pad, (long long)offset);
-	    pad = 0;
+	} else if (n == 0) {
+	    Log("1 Volser: DumpFile: Premature EOF reading inode %s for vnode %d\n",
+		PrintInode(stmp, handleP->fd_ih->ih_ino), vnode);
+	    error = VOLSERDUMPERROR;
 	}
-
-	/* If didn't read enough data, null padd the rest of the buffer. This
-	 * can happen if, for instance, the media has some bad spots. We don't
-	 * want to quit the dump, so we start null padding.
-	 */
-	if (n < howMany) {
-	    /* Record the read error */
-	    if (n < 0) {
-		n = 0;
-		Log("1 Volser: DumpFile: Error reading inode %s for vnode %d: %s\n", PrintInode(stmp, handleP->fd_ih->ih_ino), vnode, afs_error_message(errno));
-	    } else if (!pad) {
-		Log("1 Volser: DumpFile: Error reading inode %s for vnode %d\n", PrintInode(stmp, handleP->fd_ih->ih_ino), vnode);
-	    }
-
-	    /* Pad the rest of the buffer with zeros. Remember offset we started
-	     * padding. Keep total tally of padding.
-	     */
-	    memset(p + n, 0, howMany - n);
-	    if (!pad)
-		offset = (howBig - nbytes) + n;
-	    pad += (howMany - n);
-
-	    /* Now seek over the data we could not get. An error here means we
-	     * can't do the next read.
-	     */
-	    howFar = (size_t)((howBig - nbytes) + howMany);
+	if (error != 0) {
+	    break;
 	}
 
+	howFar += n;
+	nbytes -= n;
+
 	/* Now write the data out */
-	if (iod_Write(iodp, (char *)p, howMany) != howMany)
+	if (iod_Write(iodp, (char *)p, n) != n)
 	    error = VOLSERDUMPERROR;
 #ifndef AFS_PTHREAD_ENV
 	IOMGR_Poll();
 #endif
-    }
-
-    if (pad) {			/* Any padding we hadn't reported yet */
-	Log("1 Volser: DumpFile: Null padding file: %d bytes at offset %lld\n",
-	    pad, (long long)offset);
     }
 
     free(p);
