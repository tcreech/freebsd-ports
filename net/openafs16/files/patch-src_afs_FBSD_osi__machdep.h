--- src/afs/FBSD/osi_machdep.h.orig	2016-11-30 20:06:42 UTC
+++ src/afs/FBSD/osi_machdep.h
@@ -22,6 +22,9 @@
 #include <sys/lock.h>
 #include <sys/time.h>
 #include <sys/mutex.h>
+#include <sys/param.h>
+#include <sys/lock.h>
+#include <sys/sx.h>
 #include <sys/vnode.h>
 #if defined(AFS_FBSD80_ENV)
 #include <sys/priv.h>
@@ -116,31 +119,31 @@ extern void osi_fbsd_free(void *p);
 #define simple_unlock(x) mtx_unlock(x)
 #define        gop_rdwr(rw,gp,base,len,offset,segflg,unit,cred,aresid) \
   vn_rdwr((rw),(gp),(base),(len),(offset),(segflg),(unit),(cred),(cred),(aresid), curthread)
-extern struct mtx afs_global_mtx;
+extern struct sx afs_global_sx;
 extern struct thread *afs_global_owner;
 #define AFS_GLOCK() \
     do { \
-	mtx_assert(&afs_global_mtx, (MA_NOTOWNED)); \
-	mtx_lock(&afs_global_mtx); \
-	mtx_assert(&afs_global_mtx, (MA_OWNED|MA_NOTRECURSED)); \
+	sx_assert(&afs_global_sx, (SA_UNLOCKED)); \
+	sx_xlock(&afs_global_sx); \
+	sx_assert(&afs_global_sx, (SA_XLOCKED|SA_NOTRECURSED)); \
     } while (0)
 #define AFS_GUNLOCK() \
     do { \
-	mtx_assert(&afs_global_mtx, (MA_OWNED|MA_NOTRECURSED)); \
-	mtx_unlock(&afs_global_mtx); \
+	sx_assert(&afs_global_sx, (SA_XLOCKED|SA_NOTRECURSED)); \
+	sx_xunlock(&afs_global_sx); \
     } while (0)
-#define ISAFS_GLOCK() (mtx_owned(&afs_global_mtx))
+#define ISAFS_GLOCK() (sx_xlocked(&afs_global_sx))
 # if defined(AFS_FBSD80_ENV) && defined(WITNESS)
 #  define osi_InitGlock() \
 	do { \
-	    memset(&afs_global_mtx, 0, sizeof(struct mtx)); \
-	    mtx_init(&afs_global_mtx, "AFS global lock", NULL, MTX_DEF); \
+	    memset(&afs_global_sx, 0, sizeof(struct sx)); \
+	    sx_init(&afs_global_sx, "AFS global sx"); \
 	    afs_global_owner = 0; \
 	} while(0)
 # else
 #  define osi_InitGlock() \
     do { \
-	mtx_init(&afs_global_mtx, "AFS global lock", NULL, MTX_DEF); \
+	sx_init(&afs_global_sx, "AFS global sx"); \
 	afs_global_owner = 0; \
     } while (0)
 # endif
