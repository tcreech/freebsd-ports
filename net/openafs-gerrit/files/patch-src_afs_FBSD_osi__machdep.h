--- src/afs/FBSD/osi_machdep.h.orig	2020-05-01 00:46:18 UTC
+++ src/afs/FBSD/osi_machdep.h
@@ -33,6 +33,7 @@
 
 typedef struct ucred afs_ucred_t;
 typedef struct proc afs_proc_t;
+typedef afs_ucred_t cred_t;
 
 #define afs_bufferpages bufpages
 #ifndef iodone
