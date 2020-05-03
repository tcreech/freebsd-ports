--- src/afs/afs.h.orig	2020-05-01 00:46:18 UTC
+++ src/afs/afs.h
@@ -952,7 +952,7 @@ struct vcache {
     struct bhv_desc vc_bhv_desc;	/* vnode's behavior data. */
 #endif
 #endif				/* AFS_SGI_ENV */
-#if defined(AFS_LINUX26_ENV)
+#if defined(AFS_LINUX26_ENV) || defined(AFS_FBSD_ENV)
     cred_t *cred;		/* last writer's cred */
 #endif
 #ifdef AFS_LINUX24_ENV
