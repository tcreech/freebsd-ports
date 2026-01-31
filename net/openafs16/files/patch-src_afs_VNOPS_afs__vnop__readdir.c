--- src/afs/VNOPS/afs_vnop_readdir.c.orig	2018-09-11 15:52:48 UTC
+++ src/afs/VNOPS/afs_vnop_readdir.c
@@ -159,10 +159,23 @@ struct min_direct {		/* miniature direct structure */
     uint16_t d_namlen;		/* length of string in d_name */
     uint8_t  d_type; 		/* file type, see below */
 #elif defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
+#if defined(AFS_FBSD120_ENV)
+    /* FreeBSD 12.0 moved to 64-bit inodes. */
+    ino_t d_fileno;
+    off_t d_off;
+#else
     afs_uint32 d_fileno;
+#endif
     u_short d_reclen;
     u_char d_type;
+#if defined(AFS_FBSD120_ENV)
+    /* FreeBSD 12.0 also bumped d_namlen to 16 bits. */
+    u_char  d_pad0;
+    u_short d_namlen;
+    u_short d_pad1;
+#else
     u_char d_namlen;
+#endif
 #elif defined(AFS_SUN5_ENV)
     afs_uint32 d_fileno;
     afs_int32 d_off;
