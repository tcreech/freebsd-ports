--- src/viced/afsfileprocs.c.orig	2021-01-14 21:08:41 UTC
+++ src/viced/afsfileprocs.c
@@ -6046,14 +6046,6 @@ SRXAFS_SetVolumeStatus(struct rx_call * acall, afs_int
 afs_int32
 SRXAFS_GetRootVolume(struct rx_call * acall, char **VolumeName)
 {
-#ifdef notdef
-    int fd;
-    int len;
-    char *temp;
-    struct rx_connection *tcon;
-    struct host *thost;
-    Error errorCode = 0;
-#endif
     struct fsstats fsstats;
 
     fsstats_StartOp(&fsstats, FS_STATS_RPCIDX_GETROOTVOLUME);
