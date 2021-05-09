--- src/afs/afs.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs.h
@@ -46,15 +46,20 @@ extern enum afs_shutdown_state afs_shuttingdown;
  * Macros to uniquely identify the AFS vfs struct
  */
 #define	AFS_VFSMAGIC		0x1234
-#if defined(AFS_SUN5_ENV) || defined(AFS_HPUX90_ENV) || defined(AFS_LINUX20_ENV)
-#define	AFS_VFSFSID		99
+
+#if defined(UKERNEL)
+# if defined(AFS_USR_AIX_ENV) || defined(AFS_USR_SGI_ENV)
+#  define AFS_VFSFSID		AFS_MOUNT_AFS
+# else
+#  define AFS_VFSFSID		99
+# endif
+#elif defined(AFS_SUN5_ENV) || defined(AFS_HPUX90_ENV) || defined(AFS_LINUX20_ENV)
+# define AFS_VFSFSID		99
+#elif defined(AFS_SGI_ENV)
+# define AFS_VFSFSID		afs_fstype
 #else
-#if defined(AFS_SGI_ENV)
-#define AFS_VFSFSID		afs_fstype
-#else
-#define	AFS_VFSFSID		AFS_MOUNT_AFS
+# define AFS_VFSFSID		AFS_MOUNT_AFS
 #endif
-#endif
 /* use this value for reporting total space, free space, etc.
  * fake a high number to satisfy programs that use the statfs call to make sure
  * that there's enough space in the device partition before storing something
@@ -104,6 +109,8 @@ extern enum afs_shutdown_state afs_shuttingdown;
 
 #define AFS_MAXCBRSCALL	32	/* max to return in a given call (must be <= AFSCBMAX) */
 #define	VCACHE_FREE	5
+#define VCACHE_DYNAMIC_STRESSED 10000
+#define VCACHE_STRESS_LOGINTERVAL (60*60*4) /* 4 hours */
 #define	AFS_NRXPACKETS	80
 #define	AFS_RXDEADTIME	50
 #define AFS_HARDDEADTIME	120
@@ -135,6 +142,7 @@ struct sysname_info {
 #define AFS_VMSYNC_INVAL 2	/* sync and invalidate pages */
 #define AFS_LASTSTORE   4
 #define AFS_VMSYNC      8       /* sync pages but do not invalidate */
+#define AFS_NOVMSYNC    16      /* force skipping syncing vm pages; just write afs dcache data */
 
 /* background request structure */
 #define	BPARMS		4
@@ -951,8 +959,8 @@ struct vcache {
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
@@ -1330,7 +1338,7 @@ struct afs_FetchOutput {
 	avc->f.states |= CCore;	/* causes close to be called later */ \
                                                                       \
 	/* The cred and vnode holds will be released in afs_FlushActiveVcaches */  \
-	AFS_FAST_HOLD(avc);	/* So it won't disappear */           \
+	osi_Assert(osi_vnhold(avc) == 0);	/* So it won't disappear */        \
 	CRKEEP(avc, acred); /* Should use a better place for the creds */ \
     }                                                                         \
     else {                                                                    \
@@ -1354,6 +1362,9 @@ extern afs_int32 *afs_dcnextTbl;	/*Dcache hash table l
 extern afs_int32 afs_cacheFiles;	/*Size of afs_indexTable */
 extern afs_int32 afs_cacheBlocks;	/*1K blocks in cache */
 extern afs_int32 afs_cacheStats;	/*Stat entries in cache */
+extern afs_int32 afs_freeDCCount;	/*Count of elts in freeDCList */
+extern afs_uint32 afs_CacheTooFullCount;
+extern afs_uint32 afs_WaitForCacheDrainCount;
 extern struct vcache *afs_vhashT[VCSIZE];	/*Stat cache hash table */
 extern struct afs_q afs_vhashTV[VCSIZE]; /* cache hash table on volume */
 extern afs_int32 afs_initState;	/*Initialization state */
