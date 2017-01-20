--- src/afs/afs_osi.h.orig	2016-11-30 20:06:42 UTC
+++ src/afs/afs_osi.h
@@ -91,12 +91,7 @@ struct osi_dev {
 };
 
 struct afs_osi_WaitHandle {
-#ifdef AFS_FBSD_ENV
-    struct cv wh_condvar;
-    int wh_inited;
-#else
     caddr_t proc;		/* process waiting */
-#endif
 };
 
 #define	osi_SetFileProc(x,p)	((x)->proc=(p))
@@ -329,7 +324,7 @@ typedef struct timeval osi_timeval32_t;
 		AFS_GLOCK();					\
 	} while(0)
 #else
-#if defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
+#if defined(AFS_DARWIN_ENV) 
 #define AFS_UIOMOVE(SRC,LEN,RW,UIO,CODE)			\
 	do {							\
 	    int haveGlock = ISAFS_GLOCK();			\
@@ -340,6 +335,17 @@ typedef struct timeval osi_timeval32_t;
 	    if (haveGlock)					\
 		AFS_GLOCK();					\
 	} while(0)
+#elif defined(AFS_XBSD_ENV)
+#define AFS_UIOMOVE(SRC,LEN,RW,UIO,CODE)			\
+	do {							\
+	    int haveGlock = ISAFS_GLOCK();			\
+	    if (haveGlock)					\
+		AFS_GUNLOCK();					\
+	    (UIO)->uio_rw = (RW);				\
+	    CODE = vn_io_fault_uiomove((SRC),(LEN),(UIO));	\
+	    if (haveGlock)					\
+		AFS_GLOCK();					\
+	} while(0)
 #else
 #define AFS_UIOMOVE(SRC,LEN,RW,UIO,CODE)			\
 	do {							\
@@ -376,12 +382,18 @@ typedef struct timeval osi_timeval32_t;
 	    uio_setrw((UIO),(RW));				\
 	    CODE = uiomove((SRC),(LEN),(UIO));			\
 	} while(0)
-#elif defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
+#elif defined(AFS_DARWIN_ENV)
 #define AFS_UIOMOVE(SRC,LEN,RW,UIO,CODE)			\
 	do {							\
 	    (UIO)->uio_rw = (RW);				\
 	    CODE = uiomove((SRC),(LEN),(UIO));			\
 	} while(0)
+#elif defined(AFS_XBSD_ENV)
+#define AFS_UIOMOVE(SRC,LEN,RW,UIO,CODE)			\
+	do {							\
+	    (UIO)->uio_rw = (RW);				\
+	    CODE = vn_io_fault_uiomove((SRC),(LEN),(UIO));	\
+	} while(0)
 #else
 #define AFS_UIOMOVE(SRC,LEN,RW,UIO,CODE)			\
 	do {							\
