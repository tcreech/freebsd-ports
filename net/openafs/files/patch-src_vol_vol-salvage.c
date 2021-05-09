--- src/vol/vol-salvage.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/vol-salvage.c
@@ -119,13 +119,9 @@ Vnodes with 0 inode pointers in RW volumes are now del
 #endif
 #endif
 #else /* AFS_VFSINCL_ENV */
-#ifdef	AFS_OSF_ENV
-#include <ufs/inode.h>
-#else /* AFS_OSF_ENV */
 #if !defined(AFS_LINUX20_ENV) && !defined(AFS_XBSD_ENV) && !defined(AFS_DARWIN_ENV)
 #include <sys/inode.h>
 #endif
-#endif
 #endif /* AFS_VFSINCL_ENV */
 #endif /* AFS_SGI_ENV */
 #ifdef	AFS_AIX_ENV
@@ -192,9 +188,6 @@ Vnodes with 0 inode pointers in RW volumes are now del
 
 #define SALV_BUFFER_SIZE 1024
 
-#ifdef	AFS_OSF_ENV
-extern void *calloc();
-#endif
 static char *TimeStamp(char *buffer, size_t size, time_t clock, int precision);
 
 
@@ -701,7 +694,7 @@ void
 SalvageFileSys1(struct DiskPartition64 *partP, VolumeId singleVolumeNumber)
 {
     char *name, *tdir;
-    char inodeListPath[256];
+    char *inodeListPath = NULL;
     FD_t inodeFile = INVALID_FD;
     static char tmpDevName[100];
     static char wpath[100];
@@ -811,10 +804,16 @@ SalvageFileSys1(struct DiskPartition64 *partP, VolumeI
     tdir = (tmpdir ? tmpdir : salvinfo->fileSysPath);
 #ifdef AFS_NT40_ENV
     (void)_putenv("TMP=");	/* If "TMP" is set, then that overrides tdir. */
-    (void)strncpy(inodeListPath, _tempnam(tdir, "salvage.inodes."), 255);
+    inodeListPath = strdup(_tempnam(tdir, "salvage.inodes."));
+    if (inodeListPath == NULL) {
+	Abort("Error allocating memory for inodeListPath\n");
+    }
 #else
-    snprintf(inodeListPath, 255, "%s" OS_DIRSEP "salvage.inodes.%s.%d", tdir, name,
+    code = asprintf(&inodeListPath, "%s" OS_DIRSEP "salvage.inodes.%s.%d", tdir, name,
 	     getpid());
+    if (code == -1) {
+	Abort("Error allocating memory for inodeListPath\n");
+    }
 #endif
 
     inodeFile = OS_OPEN(inodeListPath, O_RDWR|O_TRUNC|O_CREAT, 0666);
@@ -843,11 +842,16 @@ SalvageFileSys1(struct DiskPartition64 *partP, VolumeI
 
     if (GetInodeSummary(salvinfo, inodeFile, singleVolumeNumber) < 0) {
 	OS_CLOSE(inodeFile);
+	free(inodeListPath);
 	return;
     }
     salvinfo->inodeFd = inodeFile;
     if (salvinfo->inodeFd == INVALID_FD)
 	Abort("Temporary file %s is missing...\n", inodeListPath);
+
+    free(inodeListPath);
+    inodeListPath = NULL;
+
     OS_SEEK(salvinfo->inodeFd, 0L, SEEK_SET);
     if (ListInodeOption) {
 	PrintInodeList(salvinfo);
@@ -4048,8 +4052,15 @@ SalvageVolume(struct SalvInfo *salvinfo, struct InodeS
 	     * link count was not incremented in JudgeEntry().
 	     */
 	    if (class == vLarge) {	/* directory vnode */
-		pv = vnodeIdToBitNumber(vep->parent);
-		if (salvinfo->vnodeInfo[vLarge].vnodes[pv].unique != 0) {
+		struct VnodeEssence *parent_vep;
+
+		parent_vep = CheckVnodeNumber(salvinfo, vep->parent);
+
+		if (!parent_vep)
+		    Log("Vnode %d has invalid or out-of-range parent vnode %d;" \
+			" ignore parent count adjustment\n",
+			ThisVnode, vep->parent);
+		else if (parent_vep->unique != 0) {
 		    if (vep->parent == 1 && newrootdir) {
 			/* this vnode's parent was the volume root, and
 			 * we just created the volume root. So, the parent
@@ -4060,7 +4071,7 @@ SalvageVolume(struct SalvInfo *salvinfo, struct InodeS
 			 /* noop */
 
 		    } else {
-			salvinfo->vnodeInfo[vLarge].vnodes[pv].count++;
+			parent_vep->count++;
 		    }
 		}
 	    }
