--- src/afs/afs.h.orig	2021-12-09 17:07:41 UTC
+++ src/afs/afs.h
@@ -142,6 +142,7 @@ struct sysname_info {
 #define AFS_VMSYNC_INVAL 2	/* sync and invalidate pages */
 #define AFS_LASTSTORE   4
 #define AFS_VMSYNC      8       /* sync pages but do not invalidate */
+#define AFS_NOVMSYNC    16      /* force skipping syncing vm pages; just write afs dcache data */
 
 /* background request structure */
 #define	BPARMS		4
@@ -958,8 +959,8 @@ struct vcache {
     struct bhv_desc vc_bhv_desc;	/* vnode's behavior data. */
 #endif
 #endif				/* AFS_SGI_ENV */
-#if defined(AFS_LINUX26_ENV)
-    cred_t *cred;		/* last writer's cred */
+#if defined(AFS_LINUX26_ENV) || defined(AFS_FBSD_ENV)
+    afs_ucred_t *cred;		/* last writer's cred */
 #endif
 #ifdef AFS_LINUX24_ENV
     struct dentry *target_link; /* dentry we prefer, when we are redirecting
