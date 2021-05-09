--- src/uss/uss_vol.h.orig	2021-01-14 21:08:41 UTC
+++ src/uss/uss_vol.h
@@ -103,19 +103,4 @@ extern afs_int32 uss_vol_GetVolInfoFromMountPoint(char
      *    Other error code if problem occurred in lower-level call.
      */
 
-#if 0
-extern afs_int32 uss_vol_DeleteMountPoint();
-    /*
-     * Summary:
-     *    Given a mountpoint, nuke it.
-     *
-     * Args:
-     *    char *a_mountpoint : Name of the mountpoint.
-     *
-     * Returns:
-     *    0 if everything went well,
-     *    1 if there was a problem in the routine itself, or
-     *    Other error code if problem occurred in lower-level call.
-     */
-#endif
 #endif /* _USS_VOL_H_ */
