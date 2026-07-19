--- src/afs/VNOPS/afs_vnop_open.c.orig	2022-12-15 20:10:23 UTC
+++ src/afs/VNOPS/afs_vnop_open.c
@@ -151,7 +151,7 @@ afs_open(struct vcache **avcp, afs_int32 aflags, afs_u
     if (writing)
 	tvc->execsOrWriters++;
     tvc->opens++;
-#if defined(AFS_SGI_ENV) || defined (AFS_LINUX_ENV)
+#if defined(AFS_SGI_ENV) || defined (AFS_LINUX_ENV) || defined(AFS_FBSD_ENV)
     if (writing && tvc->cred == NULL) {
 	crhold(acred);
 	tvc->cred = acred;
