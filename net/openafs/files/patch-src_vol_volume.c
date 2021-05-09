--- src/vol/volume.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/volume.c
@@ -45,9 +45,6 @@
 
 #ifndef AFS_NT40_ENV
 #if !defined(AFS_SGI_ENV)
-#ifdef	AFS_OSF_ENV
-#include <ufs/fs.h>
-#else /* AFS_OSF_ENV */
 #ifdef AFS_VFSINCL_ENV
 #define VFS
 #ifdef	AFS_SUN5_ENV
@@ -65,7 +62,6 @@
 #include <sys/fs.h>
 #endif
 #endif /* AFS_VFSINCL_ENV */
-#endif /* AFS_OSF_ENV */
 #endif /* AFS_SGI_ENV */
 #endif /* !AFS_NT40_ENV */
 
@@ -124,9 +120,6 @@ pthread_cond_t vol_vinit_cond;
 int vol_attach_threads = 1;
 #endif /* AFS_PTHREAD_ENV */
 
-/* start-time configurable I/O parameters */
-ih_init_params vol_io_params;
-
 #ifdef AFS_DEMAND_ATTACH_FS
 pthread_mutex_t vol_salvsync_mutex;
 
@@ -142,10 +135,6 @@ static volatile sig_atomic_t vol_disallow_salvsync = 0
  */
 static int vol_shutting_down = 0;
 
-#ifdef	AFS_OSF_ENV
-extern void *calloc(), *realloc();
-#endif
-
 /* Forward declarations */
 static Volume *attach2(Error * ec, VolumeId volumeId, char *path,
 		       struct DiskPartition64 *partp, Volume * vp,
@@ -163,9 +152,6 @@ static void ReleaseVolumeHeader(struct volHeader *hd);
 static void FreeVolumeHeader(Volume * vp);
 static void AddVolumeToHashTable(Volume * vp, VolumeId hashid);
 static void DeleteVolumeFromHashTable(Volume * vp);
-#if 0
-static int VHold(Volume * vp);
-#endif
 static int VHold_r(Volume * vp);
 static void VGetBitmap_r(Error * ec, Volume * vp, VnodeClass class);
 static void VReleaseVolumeHandles_r(Volume * vp);
@@ -3757,18 +3743,6 @@ VOfflineTimeout(struct timespec *ats)
 
 #endif /* !AFS_PTHREAD_ENV */
 
-#if 0
-static int
-VHold(Volume * vp)
-{
-    int retVal;
-    VOL_LOCK;
-    retVal = VHold_r(vp);
-    VOL_UNLOCK;
-    return retVal;
-}
-#endif
-
 static afs_int32
 VIsGoingOffline_r(struct Volume *vp)
 {
@@ -4024,15 +3998,6 @@ GetVolume(Error * ec, Error * client_ec, VolumeId volu
           const struct timespec *timeout)
 {
     Volume *vp = hint;
-    /* pull this profiling/debugging code out of regular builds */
-#ifdef notdef
-#define VGET_CTR_INC(x) x++
-    unsigned short V0 = 0, V1 = 0, V2 = 0, V3 = 0, V5 = 0, V6 =
-	0, V7 = 0, V8 = 0, V9 = 0;
-    unsigned short V10 = 0, V11 = 0, V12 = 0, V13 = 0, V14 = 0, V15 = 0;
-#else
-#define VGET_CTR_INC(x)
-#endif
 #ifdef AFS_DEMAND_ATTACH_FS
     Volume *avp, * rvp = hint;
 #endif
@@ -4061,7 +4026,6 @@ GetVolume(Error * ec, Error * client_ec, VolumeId volu
 	*ec = 0;
 	if (client_ec)
 	    *client_ec = 0;
-	VGET_CTR_INC(V0);
 
 	vp = VLookupVolume_r(ec, volumeId, vp);
 	if (*ec) {
@@ -4078,9 +4042,7 @@ GetVolume(Error * ec, Error * client_ec, VolumeId volu
 #endif /* AFS_DEMAND_ATTACH_FS */
 
 	if (!vp) {
-	    VGET_CTR_INC(V1);
 	    if (VInit < 2) {
-		VGET_CTR_INC(V2);
 		/* Until we have reached an initialization level of 2
 		 * we don't know whether this volume exists or not.
 		 * We can't sleep and retry later because before a volume
@@ -4095,7 +4057,6 @@ GetVolume(Error * ec, Error * client_ec, VolumeId volu
 	    break;
 	}
 
-	VGET_CTR_INC(V3);
 	IncUInt64(&VStats.hdr_gets);
 
 #ifdef AFS_DEMAND_ATTACH_FS
@@ -4231,7 +4192,6 @@ GetVolume(Error * ec, Error * client_ec, VolumeId volu
 
 	LoadVolumeHeader(ec, vp);
 	if (*ec) {
-	    VGET_CTR_INC(V6);
 	    /* Only log the error if it was a totally unexpected error.  Simply
 	     * a missing inode is likely to be caused by the volume being deleted */
 	    if (errno != ENXIO || GetLogLevel() != 0)
@@ -4251,21 +4211,17 @@ GetVolume(Error * ec, Error * client_ec, VolumeId volu
 	    break;
 	}
 
-	VGET_CTR_INC(V7);
 	if (vp->shuttingDown) {
-	    VGET_CTR_INC(V8);
 	    *ec = VNOVOL;
 	    vp = NULL;
 	    break;
 	}
 
 	if (programType == fileServer) {
-	    VGET_CTR_INC(V9);
 	    if (vp->goingOffline) {
 		if (timeout && VTimedOut(timeout)) {
 		    /* we've timed out; don't wait for the vol */
 		} else {
-		    VGET_CTR_INC(V10);
 #ifdef AFS_DEMAND_ATTACH_FS
 		    /* wait for the volume to go offline */
 		    if (V_attachState(vp) == VOL_STATE_GOING_OFFLINE) {
@@ -4283,21 +4239,15 @@ GetVolume(Error * ec, Error * client_ec, VolumeId volu
 		}
 	    }
 	    if (vp->specialStatus) {
-		VGET_CTR_INC(V11);
 		*ec = vp->specialStatus;
 	    } else if (V_inService(vp) == 0 || V_blessed(vp) == 0) {
-		VGET_CTR_INC(V12);
 		*ec = VNOVOL;
 	    } else if (V_inUse(vp) == 0 || vp->goingOffline) {
-		VGET_CTR_INC(V13);
 		*ec = VOFFLINE;
-	    } else {
-		VGET_CTR_INC(V14);
 	    }
 	}
 	break;
     }
-    VGET_CTR_INC(V15);
 
 #ifdef AFS_DEMAND_ATTACH_FS
     /* if no error, bump nUsers */
@@ -4776,16 +4726,6 @@ VDetachVolume_r(Error * ec, Volume * vp)
 	 * which the file server would attempt to put on line
 	 */
 	FSYNC_VolOp(volume, tpartp->name, useDone, 0, NULL);
-	/* XXX this code path is only hit by volume utilities, thus
-	 * V_BreakVolumeCallbacks will always be NULL.  if we really
-	 * want to break callbacks in this path we need to use FSYNC_VolOp() */
-#ifdef notdef
-	/* Dettaching it so break all callbacks on it */
-	if (V_BreakVolumeCallbacks) {
-	    Log("volume %u detached; breaking all call backs\n", volume);
-	    (*V_BreakVolumeCallbacks) (volume);
-	}
-#endif
     }
 #endif /* FSSYNC_BUILD_CLIENT */
 }
