--- src/volser/vol-dump.c.orig	2021-01-14 21:08:41 UTC
+++ src/volser/vol-dump.c
@@ -53,6 +53,7 @@
 
 int VolumeChanged;		/* needed by physio - leave alone */
 int verbose = 0;
+static int enable_padding; /* Pad errors with NUL bytes */
 
 /* Forward Declarations */
 static void HandleVolume(struct DiskPartition64 *partP, char *name,
@@ -145,15 +146,6 @@ handleit(struct cmd_syndesc *as, void *arock)
     afs_int32 code;
 
 
-#ifndef AFS_NT40_ENV
-#if 0
-    if (geteuid() != 0) {
-	fprintf(stderr, "voldump must be run as root; sorry\n");
-	exit(1);
-    }
-#endif
-#endif
-
     if ((ti = as->parms[0].items))
 	partName = ti->data;
     if ((ti = as->parms[1].items))
@@ -170,6 +162,9 @@ handleit(struct cmd_syndesc *as, void *arock)
 		return code;
 	}
     }
+    if (as->parms[5].items != NULL) { /* -pad-errors */
+	enable_padding = 1;
+    }
 
     DInit(10);
 
@@ -280,6 +275,8 @@ main(int argc, char **argv)
     cmd_AddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL,
 		"Trace dump progress (very verbose)");
     cmd_AddParm(ts, "-time", CMD_SINGLE, CMD_OPTIONAL, "dump from time");
+    cmd_AddParm(ts, "-pad-errors", CMD_FLAG, CMD_OPTIONAL,
+		"pad i/o errors with NUL bytes");
     code = cmd_Dispatch(argc, argv);
     return code;
 }
@@ -536,11 +533,11 @@ DumpByteString(int dumpfd, char tag, byte * bs, int nb
 static int
 DumpFile(int dumpfd, int vnode, FdHandle_t * handleP,  struct VnodeDiskObject *v)
 {
-    int code = 0, failed_seek = 0, failed_write = 0;
+    int code = 0;
     afs_int32 pad = 0;
     afs_foff_t offset = 0;
     afs_sfsize_t nbytes, howBig;
-    ssize_t n;
+    ssize_t n = 0;
     size_t howMany;
     afs_foff_t howFar = 0;
     byte *p;
@@ -609,13 +606,11 @@ DumpFile(int dumpfd, int vnode, FdHandle_t * handleP, 
     }
 
     /* loop through whole file, while we still have bytes left, and no errors, in chunks of howMany bytes */
-    for (nbytes = size; (nbytes && !failed_write); nbytes -= howMany) {
+    for (nbytes = size; (nbytes && !code); ) {
 	if (nbytes < howMany)
 	    howMany = nbytes;
 
-	/* Read the data - unless we know we can't */
-	n = (failed_seek ? 0 : FDH_PREAD(handleP, p, howMany, howFar));
-	howFar += n;
+	n = FDH_PREAD(handleP, p, howMany, howFar);
 
 	/* If read any good data and we null padded previously, log the
 	 * amount that we had null padded.
@@ -626,42 +621,48 @@ DumpFile(int dumpfd, int vnode, FdHandle_t * handleP, 
 	    pad = 0;
 	}
 
-	/* If didn't read enough data, null padd the rest of the buffer. This
-	 * can happen if, for instance, the media has some bad spots. We don't
-	 * want to quit the dump, so we start null padding.
-	 */
-	if (n < howMany) {
-
-		if (verbose) fprintf(stderr, "  read %u instead of %u bytes.\n", (unsigned)n, (unsigned)howMany);
-
-	    /* Record the read error */
-	    if (n < 0) {
-		n = 0;
-		fprintf(stderr, "Error %d reading inode %s for vnode %d\n",
-			errno, PrintInode(stmp, handleP->fd_ih->ih_ino),
-			vnode);
-	    } else if (!pad) {
-		fprintf(stderr, "Error reading inode %s for vnode %d\n",
+	if (n < 0) {
+	    fprintf(stderr, "Error %d reading inode %s for vnode %d\n",
+		    errno, PrintInode(stmp, handleP->fd_ih->ih_ino),
+		    vnode);
+	    code = VOLSERDUMPERROR;
+	}
+	if (n == 0) {
+	    if (pad == 0) {
+		fprintf(stderr, "Unexpected EOF reading inode %s for vnode %d\n",
 			PrintInode(stmp, handleP->fd_ih->ih_ino), vnode);
 	    }
+	    code = VOLSERDUMPERROR;
+	}
 
-	    /* Pad the rest of the buffer with zeros. Remember offset we started
-	     * padding. Keep total tally of padding.
+	if (code != 0 && enable_padding) {
+	    /*
+	     * If our read failed, NUL-pad the rest of the buffer. This can
+	     * happen if, for instance, the media has some bad spots. We don't
+	     * want to quit the dump, so we start NUL padding.
 	     */
-	    memset(p + n, 0, howMany - n);
+	    memset(p, 0, howMany);
+
+	    /* Remember the offset where we started padding, and keep a total
+	     * tally of how much padding we've done. */
 	    if (!pad)
-		offset = (howBig - nbytes) + n;
-	    pad += (howMany - n);
+		offset = howFar;
+	    pad += howMany;
 
-	    /* Now seek over the data we could not get. An error here means we
-	     * can't do the next read.
-	     */
-	    howFar = ((size - nbytes) + howMany);
+	    /* Pretend we read 'howMany' bytes. */
+	    n = howMany;
+	    code = 0;
 	}
+	if (code != 0) {
+	    break;
+	}
 
+	howFar += n;
+	nbytes -= n;
+
 	/* Now write the data out */
-	if (write(dumpfd, (char *)p, howMany) != howMany)
-	    failed_write = VOLSERDUMPERROR;
+	if (write(dumpfd, (char *)p, n) != n)
+	    code = VOLSERDUMPERROR;
     }
 
     if (pad) {			/* Any padding we hadn't reported yet */
@@ -670,7 +671,7 @@ DumpFile(int dumpfd, int vnode, FdHandle_t * handleP, 
     }
 
     free(p);
-    return failed_write;
+    return code;
 }
 
 
