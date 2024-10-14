--- src/afs/afs.h.orig	2022-12-15 20:10:23 UTC
+++ src/afs/afs.h
@@ -142,6 +142,7 @@ struct sysname_info {
 #define AFS_VMSYNC_INVAL 2	/* sync and invalidate pages */
 #define AFS_LASTSTORE   4
 #define AFS_VMSYNC      8       /* sync pages but do not invalidate */
+#define AFS_NOVMSYNC    16      /* force skipping syncing vm pages; just write afs dcache data */
 
 /* background request structure */
 #define	BPARMS		4
@@ -959,8 +960,8 @@ struct vcache {
     struct bhv_desc vc_bhv_desc;	/* vnode's behavior data. */
 #endif
 #endif				/* AFS_SGI_ENV */
-#if defined(AFS_LINUX_ENV)
-    cred_t *cred;		/* last writer's cred */
+#if defined(AFS_LINUX_ENV) || defined(AFS_FBSD_ENV)
+    afs_ucred_t *cred;		/* last writer's cred */
 #endif
 #ifdef AFS_LINUX_ENV
     struct dentry *target_link; /* dentry we prefer, when we are redirecting
