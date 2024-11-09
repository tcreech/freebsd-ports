--- src/afs/afs.h.orig	2024-10-03 22:32:45 UTC
+++ src/afs/afs.h
@@ -155,6 +155,7 @@ struct dcache;
 #define AFS_VMSYNC_INVAL 2	/* sync and invalidate pages */
 #define AFS_LASTSTORE   4
 #define AFS_VMSYNC      8       /* sync pages but do not invalidate */
+#define AFS_NOVMSYNC    16      /* force skipping syncing vm pages; just write afs dcache data */
 
 /* background request structure */
 #define	BPARMS		4
@@ -968,8 +969,8 @@ struct vcache {
     struct cred *cred;		/* last writer's cred */
     struct bhv_desc vc_bhv_desc;	/* vnode's behavior data. */
 #endif				/* AFS_SGI_ENV */
-#if defined(AFS_LINUX_ENV)
-    cred_t *cred;		/* last writer's cred */
+#if defined(AFS_LINUX_ENV) || defined(AFS_FBSD_ENV)
+    afs_ucred_t *cred;		/* last writer's cred */
 #endif
 #ifdef AFS_LINUX_ENV
     struct dentry *target_link; /* dentry we prefer, when we are redirecting
