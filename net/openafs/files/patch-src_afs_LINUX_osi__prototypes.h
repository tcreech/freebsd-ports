--- src/afs/LINUX/osi_prototypes.h.orig	2021-12-09 17:07:41 UTC
+++ src/afs/LINUX/osi_prototypes.h
@@ -50,7 +50,7 @@ extern int osi_lookupname(char *aname, uio_seg_t seg, 
 			  struct dentry **dpp);
 extern int osi_abspath(char *aname, char *buf, int buflen,
 		       int followlink, char **pathp);
-extern void afs_start_thread(void (*proc)(void), char *name);
+extern void afs_start_thread(void (*proc)(void), char *name, int needs_glock);
 
 /* osi_probe.c */
 extern void *osi_find_syscall_table(int which);
