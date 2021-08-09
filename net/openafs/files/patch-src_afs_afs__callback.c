--- src/afs/afs_callback.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/afs_callback.c
@@ -447,9 +447,6 @@ loop1:
 			    afs_osi_Sleep(&tvc->f.states);
 			    goto loop1;
 			}
-#if     defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV)  || defined(AFS_HPUX_ENV) || defined(AFS_LINUX20_ENV)
-			AFS_FAST_HOLD(tvc);
-#else
 #ifdef AFS_DARWIN80_ENV
 			if (tvc->f.states & CDeadVnode) {
 			    ReleaseReadLock(&afs_xvcache);
@@ -466,9 +463,10 @@ loop1:
 			    continue;
 			}
 #else
-			AFS_FAST_HOLD(tvc);
+			if (osi_vnhold(tvc) != 0) {
+			    continue;
+			}
 #endif
-#endif
 			ReleaseReadLock(&afs_xvcache);
 			afs_StaleVCacheFlags(tvc, 0, CUnique | CBulkFetching);
 			afs_allCBs++;
@@ -530,9 +528,6 @@ loop2:
 			afs_osi_Sleep(&tvc->f.states);
 			goto loop2;
 		    }
-#if     defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV)  || defined(AFS_HPUX_ENV) || defined(AFS_LINUX20_ENV)
-		    AFS_FAST_HOLD(tvc);
-#else
 #ifdef AFS_DARWIN80_ENV
 		    if (tvc->f.states & CDeadVnode) {
 			ReleaseReadLock(&afs_xvcache);
@@ -549,9 +544,10 @@ loop2:
 			continue;
 		    }
 #else
-		    AFS_FAST_HOLD(tvc);
+		    if (osi_vnhold(tvc) != 0) {
+			continue;
+		    }
 #endif
-#endif
 		    ReleaseReadLock(&afs_xvcache);
 		    afs_StaleVCacheFlags(tvc, 0, CUnique | CBulkFetching);
 		    afs_Trace3(afs_iclSetp, CM_TRACE_CALLBACK,
@@ -984,7 +980,9 @@ afs_RXCallBackServer(void)
     /*
      * Donate this process to Rx.
      */
+    AFS_GUNLOCK();
     rx_ServerProc(NULL);
+    AFS_GLOCK();
     return (0);
 
 }				/*afs_RXCallBackServer */
