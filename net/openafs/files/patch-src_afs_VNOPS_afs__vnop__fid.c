--- src/afs/VNOPS/afs_vnop_fid.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/VNOPS/afs_vnop_fid.c
@@ -113,7 +113,7 @@ afs_fid(OSI_VC_DECL(avc), struct fid **fidpp)
 	addr[0] = (long)avc;
 #ifndef AFS_AIX41_ENV
 	/* No post processing, so don't hold ref count. */
-	AFS_FAST_HOLD(avc);
+	osi_Assert(osi_vnhold(avc) == 0);
 #endif
     }
 #if	defined(AFS_AIX_ENV) || defined(AFS_SUN5_ENV)
