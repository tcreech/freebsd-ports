--- src/afs/afs.h.orig	2021-07-29 10:24:31 UTC
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
@@ -1337,7 +1338,7 @@ struct afs_FetchOutput {
 	avc->f.states |= CCore;	/* causes close to be called later */ \
                                                                       \
 	/* The cred and vnode holds will be released in afs_FlushActiveVcaches */  \
-	AFS_FAST_HOLD(avc);	/* So it won't disappear */           \
+	osi_Assert(osi_vnhold(avc) == 0);	/* So it won't disappear */        \
 	CRKEEP(avc, acred); /* Should use a better place for the creds */ \
     }                                                                         \
     else {                                                                    \
