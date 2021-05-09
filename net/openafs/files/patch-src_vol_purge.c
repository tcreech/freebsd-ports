--- src/vol/purge.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/purge.c
@@ -46,10 +46,6 @@
 /* forward declarations */
 static int ObliterateRegion(Volume * avp, VnodeClass aclass, StreamHandle_t * afile,
 			    afs_foff_t * aoffset);
-#if 0
-static void PurgeIndex(Volume * vp, VnodeClass class);
-static void PurgeHeader(Volume * vp);
-#endif
 
 static void PurgeIndex_r(Volume * vp, VnodeClass class);
 static void PurgeHeader_r(Volume * vp);
@@ -164,17 +160,7 @@ ObliterateRegion(Volume * avp, VnodeClass aclass, Stre
     return -1;
 }
 
-#if 0
 static void
-PurgeIndex(Volume * vp, VnodeClass class)
-{
-    VOL_LOCK;
-    PurgeIndex_r(vp, class);
-    VOL_UNLOCK;
-}
-#endif
-
-static void
 PurgeIndex_r(Volume * vp, VnodeClass class)
 {
     StreamHandle_t *ifile;
@@ -203,16 +189,6 @@ PurgeIndex_r(Volume * vp, VnodeClass class)
     STREAM_CLOSE(ifile);
     FDH_CLOSE(fdP);
 }
-
-#if 0
-static void
-PurgeHeader(Volume * vp)
-{
-    VOL_LOCK;
-    PurgeHeader_r(vp);
-    VOL_UNLOCK;
-}
-#endif
 
 static void
 PurgeHeader_r(Volume * vp)
