--- src/afs/LINUX/osi_module.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_module.c
@@ -56,6 +56,12 @@ afs_init(void)
 #endif
 
     osi_Init();
+
+    /* Initialize CellLRU since it is used while traversing CellServDB proc
+     * entry */
+
+    QInit(&CellLRU);
+
 #if !defined(AFS_NONFSTRANS)
     osi_linux_nfssrv_init();
 #endif
