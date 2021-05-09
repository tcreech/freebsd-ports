--- src/afs/OBSD/osi_misc.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/OBSD/osi_misc.c
@@ -135,34 +135,6 @@ osi_obsd_Free(void *p, size_t asize)
 }
 #endif
 
-#if 0 /* XXX */
-/* I speculate this usage may be more correct than definitions
- * in afs_osi_alloc.c, which I discarded successfully for FreeBSD 7+,
- * and am trying to discard for NetBSD 4.x, but until tested, I'm
- * not rocking the boat.  Matt.
- */
-   
-void
-osi_FreeLargeSpace(void *p)
-{
-  osi_obsd_Free(p, 0);
-}
-
-void
-osi_FreeSmallSpace(void *p)
-{
-  osi_obsd_Free(p, 0);
-}
-
-void *
-osi_AllocLargeSpace(size_t size)
-{
-  AFS_ASSERT_GLOCK();
-  AFS_STATCNT(osi_AllocLargeSpace);
-  return (osi_obsd_Alloc(size, 1));
-}
-#endif
-
 int
 afs_syscall_icreate(dev, near_inode, param1, param2, param3, param4, retval)
      long *retval;
