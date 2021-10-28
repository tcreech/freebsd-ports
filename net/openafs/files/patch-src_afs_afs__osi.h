--- src/afs/afs_osi.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/afs_osi.h
@@ -149,6 +149,15 @@ extern void osi_PrePopulateVCache(struct vcache *);
 extern void osi_PostPopulateVCache(struct vcache *);
 extern void osi_AttachVnode(struct vcache *, int seq);
 
+/**
+ * Increment the refcount on the given vcache.
+ *
+ * @retval 0 Success
+ * @retval nonzero Error obtaining reference; the vcache is no longer valid and
+ *		   the caller should act as if it doesn't exist.
+ */
+extern int osi_vnhold(struct vcache *);
+
 /*
  * In IRIX 6.5 and NetBSD we cannot have DEBUG turned on since certain
  * system-defined structures are a different size with DEBUG on, the
@@ -266,9 +275,6 @@ typedef struct timeval osi_timeval32_t;
  * (Also, of course, the vnode is assumed to be one of ours.  Can't use this
  * macro for V-file vnodes.)
  */
-/* osi_vnhold is defined in PLATFORM/osi_machdep.h */
-#define AFS_FAST_HOLD(vp) osi_vnhold((vp), 0)
-
 #ifdef AFS_AIX_ENV
 #define AFS_FAST_RELE(vp) VREFCOUNT_DEC(vp)
 #else
