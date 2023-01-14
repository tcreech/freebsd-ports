--- src/afs/afs_daemons.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/afs_daemons.c
@@ -223,19 +223,21 @@ afs_Daemon(void)
 	    }
 	}
 	if (last10MinCheck + 600 < now) {
+	    int addrs_changed;
 #ifdef AFS_USERSPACE_IP_ADDR
 	    extern int rxi_GetcbiInfo(void);
 #endif
 	    afs_Trace1(afs_iclSetp, CM_TRACE_PROBEUP, ICL_TYPE_INT32, 600);
 #ifdef AFS_USERSPACE_IP_ADDR
-	    if (rxi_GetcbiInfo()) {	/* addresses changed from last time */
+	    addrs_changed = rxi_GetcbiInfo();
+#else
+	    AFS_GUNLOCK();
+	    addrs_changed = rxi_GetIFInfo();
+	    AFS_GLOCK();
+#endif
+	    if (addrs_changed) {	/* addresses changed from last time */
 		afs_FlushCBs();
 	    }
-#else /* AFS_USERSPACE_IP_ADDR */
-	    if (rxi_GetIFInfo()) {	/* addresses changed from last time */
-		afs_FlushCBs();
-	    }
-#endif /* else AFS_USERSPACE_IP_ADDR */
 	    if (!afs_CheckServerDaemonStarted)
 		afs_CheckServers(0, NULL);
 	    afs_GCUserData();	/* gc old conns */
