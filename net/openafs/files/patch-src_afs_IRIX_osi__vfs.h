--- src/afs/IRIX/osi_vfs.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/IRIX/osi_vfs.h
@@ -176,10 +176,6 @@ extern int afs_is_numa_arch;
 extern int afs_is_numa_arch;
 typedef struct bhv_head1 {
     struct bhv_desc *bh_first;	/* first behavior in chain */
-#ifdef notdef
-    /* This is not present in the non NUMA machines. */
-    mrlock_t bh_mrlock;		/* lock for ops-in-progress synch. */
-#endif
 } bhv_head1_t;
 
 typedef struct vnode1 {
