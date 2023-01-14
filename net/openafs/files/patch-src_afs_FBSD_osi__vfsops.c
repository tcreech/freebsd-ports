--- src/afs/FBSD/osi_vfsops.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/FBSD/osi_vfsops.c
@@ -15,7 +15,11 @@
 
 struct vcache *afs_globalVp = NULL;
 struct mount *afs_globalVFS = NULL;
+#if defined(AFS_FBSD_UMA_BUFS)
+uma_zone_t afs_pbuf_zone;
+#else
 int afs_pbuf_freecnt = -1;
+#endif
 
 extern int Afs_xsetgroups();
 extern int afs_xioctl();
@@ -46,7 +50,11 @@ afs_init(struct vfsconf *vfc)
 	return code;
     }
     osi_Init();
+#if defined(AFS_FBSD_UMA_BUFS)
+    afs_pbuf_zone = pbuf_zsecond_create("afspbuf", nswbuf / 2);
+#else
     afs_pbuf_freecnt = nswbuf / 2 + 1;
+#endif
     return 0;
 }
 
@@ -56,6 +64,10 @@ afs_uninit(struct vfsconf *vfc)
     if (afs_globalVFS)
 	return EBUSY;
 
+#if defined(AFS_FBSD_UMA_BUFS)
+    uma_zdestroy(afs_pbuf_zone);
+#endif
+
     return syscall_helper_unregister(afs_syscalls);
 }
 
@@ -232,17 +244,20 @@ tryagain:
 
 	ASSERT_VI_UNLOCKED(vp, "afs_root");
 	AFS_GUNLOCK();
+#ifdef AFS_FBSD_VGET_NOTHREAD
+	error = vget(vp, LK_EXCLUSIVE | LK_RETRY);
+#else
 	error = vget(vp, LK_EXCLUSIVE | LK_RETRY, td);
+#endif /* AFS_FBSD_VGET_NOTHREAD */
 	AFS_GLOCK();
+	if (error != 0)
+	    goto tryagain;
 	/* we dropped the glock, so re-check everything it had serialized */
 	if (!afs_globalVp || !(afs_globalVp->f.states & CStatd) ||
 		tvp != afs_globalVp) {
 	    vput(vp);
-	    afs_PutVCache(tvp);
 	    goto tryagain;
 	}
-	if (error != 0)
-	    goto tryagain;
 	/*
 	 * I'm uncomfortable about this.  Shouldn't this happen at a
 	 * higher level, and shouldn't we busy the top-level directory
