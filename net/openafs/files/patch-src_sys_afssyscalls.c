--- src/sys/afssyscalls.c.orig	2021-01-14 21:08:41 UTC
+++ src/sys/afssyscalls.c
@@ -43,10 +43,6 @@ static void check_iops(int index, char *fun, char *fil
 #pragma weak xiinc = iinc
 #pragma weak xidec = idec
 #pragma weak xiopen = iopen
-#ifdef notdef
-#pragma weak xiread = iread
-#pragma weak xiwrite = iwrite
-#endif
 
 int
 icreate(int dev, int near_inode, int param1, int param2, int param3,
@@ -170,23 +166,6 @@ afs_init_kernel_config(int flag)
 }
 #endif
 
-#ifdef notdef
-/* iread and iwrite are deprecated interfaces. Use inode_read and inode_write instead. */
-int
-iread(int dev, int inode, int inode_p1, unsigned int offset, char *cbuf,
-      unsigned int count)
-{
-    return (syscall(AFS_IREAD, dev, inode, inode_p1, offset, cbuf, count));
-}
-
-int
-iwrite(int dev, int inode, int inode_p1, unsigned int offset, char *cbuf,
-       unsigned int count)
-{
-    return (syscall(AFS_IWRITE, dev, inode, inode_p1, offset, cbuf, count));
-}
-#endif /* notdef */
-
 #else /* AFS_SGI_ENV */
 
 #ifndef AFS_NAMEI_ENV
@@ -249,39 +228,6 @@ idec(int dev, int inode, int inode_p1)
     return (errcode);
 }
 
-
-#ifdef notdef
-int
-iread(int dev, int inode, int inode_p1, unsigned int offset, char *cbuf,
-      unsigned int count)
-{
-    int errcode;
-    struct iparam iparams;
-
-    iparams.param1 = inode_p1;
-    iparams.param2 = offset;
-    iparams.param3 = (long)cbuf;
-    iparams.param4 = count;
-    errcode = syscall(AFS_SYSCALL, AFSCALL_IREAD, dev, inode, &iparams);
-    return (errcode);
-}
-
-
-iwrite(int dev, int inode, int inode_p1, unsigned int offset, char *cbuf,
-       unsigned int count)
-{
-    int errcode;
-    struct iparam iparams;
-
-    iparams.param1 = inode_p1;
-    iparams.param2 = offset;
-    iparams.param3 = (long)cbuf;
-    iparams.param4 = count;
-
-    errcode = syscall(AFS_SYSCALL, AFSCALL_IWRITE, dev, inode, &iparams);
-    return (errcode);
-}
-#endif
 
 #endif /* AFS_NAMEI_ENV */
 
