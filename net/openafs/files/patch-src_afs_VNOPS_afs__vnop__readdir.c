--- src/afs/VNOPS/afs_vnop_readdir.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/VNOPS/afs_vnop_readdir.c
@@ -158,6 +158,15 @@ struct min_direct {		/* miniature direct structure */
     uint16_t d_reclen;		/* length of this record */
     uint16_t d_namlen;		/* length of string in d_name */
     uint8_t  d_type; 		/* file type, see below */
+#elif defined(AFS_FBSD120_ENV)
+    /* FreeBSD 12.0 moved to 64-bit inodes and bumped d_namlen to 16 bits. */
+    ino_t d_fileno;
+    off_t d_off;
+    u_short d_reclen;
+    u_char d_type;
+    u_char  d_pad0;
+    u_short d_namlen;
+    u_short d_pad1;
 #elif defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
     afs_uint32 d_fileno;
     u_short d_reclen;
@@ -317,6 +326,7 @@ afs_readdir_move(struct DirEntry *de, struct vcache *v
 #endif /* AFS_SUN5_ENV */
 #ifndef	AFS_SGI53_ENV
     struct min_direct sdirEntry;
+    memset(&sdirEntry, 0, sizeof(sdirEntry));
 #endif /* AFS_SGI53_ENV */
 
     AFS_STATCNT(afs_readdir_move);
