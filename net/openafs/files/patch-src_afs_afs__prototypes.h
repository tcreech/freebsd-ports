--- src/afs/afs_prototypes.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_prototypes.h
@@ -265,6 +265,7 @@ extern struct dcache *afs_FindDCache(struct vcache *av
 extern void afs_StoreWarn(afs_int32 acode, afs_int32 avolume,
 			  afs_int32 aflags);
 extern void afs_MaybeWakeupTruncateDaemon(void);
+extern void afs_MaybeWaitForCacheDrain(void);
 extern void afs_CacheTruncateDaemon(void);
 extern void afs_AdjustSize(struct dcache *adc,
 			   afs_int32 newSize);
@@ -914,6 +915,10 @@ extern void afs_GetCMStat(char **ptr, unsigned *size);
 extern void afs_AddToMean(struct afs_MeanStats *oldMean, afs_int32 newValue);
 #endif
 
+/* ARCH/osi_kstat.c */
+extern void afs_kstat_init(void);
+extern void afs_kstat_shutdown(void);
+
 /* afs_syscall.c */
 #ifdef AFS_DARWIN100_ENV
 extern int copyin_afs_ioctl(user_addr_t cmarg, struct afs_ioctl *dst);
@@ -924,9 +929,6 @@ extern int copyin_afs_ioctl(caddr_t cmarg, struct afs_
 #if defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
 #ifdef AFS_DARWIN100_ENV
 extern int afs3_syscall(afs_proc_t *p, void *args, unsigned int *retval);
-#elif (defined(AFS_FBSD90_ENV) || defined(AFS_FBSD82_ENV)) && (__FreeBSD_version < 900044)
-/* afs3_syscall prototype is in sys/sysproto.h
-   Yes, they put it in, then took it out again (renamed with a sys_ prefix) */
 #elif defined(AFS_FBSD_ENV)
 extern int afs3_syscall(struct thread *p, void *args);
 #elif defined(AFS_NBSD50_ENV)
@@ -1051,6 +1053,7 @@ extern void afs_WarnENOSPC(void);
 /* afs_vcache.c */
 extern int VCHash(struct VenusFid *fid);
 extern int VCHashV(struct VenusFid *fid);
+extern int afs_VCacheStressed(void);
 extern int afs_ShakeLooseVCaches(afs_int32 anumber);
 extern afs_int32 afs_maxvcount;
 extern afs_int32 afs_vcount;
