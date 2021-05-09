--- src/util/afsutil_prototypes.h.orig	2021-01-14 21:08:41 UTC
+++ src/util/afsutil_prototypes.h
@@ -105,16 +105,6 @@ extern afs_int32 ktime_InterpretDate(struct ktime_date
 /* pthread_glock.c */
 
 
-/* pthread_threadname.c */
-#if defined(AFS_PTHREAD_ENV) && !defined(AFS_NT40_ENV)
-extern void afs_pthread_setname_self(const char *threadname);
-#else
-/* Allow unconditional references to afs_pthread_setname_self to
- * reduce #ifdef spaghetti.
- */
-#define afs_pthread_setname_self(threadname) (void)0
-#endif
-
 /* readdir_nt.c */
 
 
