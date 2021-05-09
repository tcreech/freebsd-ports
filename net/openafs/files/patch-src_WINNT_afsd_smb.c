--- src/WINNT/afsd/smb.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/smb.c
@@ -3908,13 +3908,7 @@ long smb_ReceiveNegotiate(smb_vc_t *vcp, smb_packet_t 
     }
 
     lock_ObtainMutex(&vcp->mx);
-#if 0
-    if (VistaProtoIndex != -1) {
-        protoIndex = VistaProtoIndex;
-        vcp->flags |= (SMB_VCFLAG_USENT | SMB_VCFLAG_USEV3);
-    } else
-#endif
-	if (NTProtoIndex != -1) {
+    if (NTProtoIndex != -1) {
         protoIndex = NTProtoIndex;
         vcp->flags |= (SMB_VCFLAG_USENT | SMB_VCFLAG_USEV3);
     }
@@ -9846,36 +9840,6 @@ void smb_Listener(void *parmp)
             lock_ReleaseMutex(&smb_StartedLock);
 	    break;
 	}
-#if 0
-        else if (code != 0) {
-            char tbuffer[AFSPATHMAX];
-
-            /* terminate silently if shutdown flag is set */
-            while (!lock_TryMutex(&smb_StartedLock)) {
-                if (smb_ListenerState == SMB_LISTENER_STOPPED || smbShutdownFlag == 1)
-                    goto exit_thread;
-                Sleep(50);
-            }
-
-            osi_Log3(smb_logp,
-                     "NCBLISTEN lana=%d failed with code %d [%s]",
-                     ncbp->ncb_lana_num, code, ncb_error_string(code));
-            osi_Log0(smb_logp,
-                     "Client exiting due to network failure. Please restart client.\n");
-
-            sprintf(tbuffer,
-                     "Client exiting due to network failure.  Please restart client.\n"
-                     "NCBLISTEN lana=%d failed with code %d [%s]",
-                     ncbp->ncb_lana_num, code, ncb_error_string(code));
-            if (showErrors)
-                code = (*smb_MBfunc)(NULL, tbuffer, "AFS Client Service: Fatal Error",
-                                      MB_OK|MB_SERVICE_NOTIFICATION);
-            osi_panic(tbuffer, __FILE__, __LINE__);
-
-            lock_ReleaseMutex(&smb_StartedLock);
-            break;
-        }
-#endif /* 0 */
 
         /* a successful packet received.  clear bridge error count */
         bridgeCount = 0;
