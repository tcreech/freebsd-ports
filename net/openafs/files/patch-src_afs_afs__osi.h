--- src/afs/afs_osi.h.orig	2016-12-08 04:01:51 UTC
+++ src/afs/afs_osi.h
@@ -98,12 +98,7 @@ struct osi_dev {
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
