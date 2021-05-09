--- src/WINNT/afsd/smb3.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/smb3.c
@@ -3766,10 +3766,6 @@ long smb_ReceiveTran2QPathInfo(smb_vc_t *vcp, smb_tran
 /* TRANS2_SET_PATH_INFORMATION */
 long smb_ReceiveTran2SetPathInfo(smb_vc_t *vcp, smb_tran2Packet_t *p, smb_packet_t *opx)
 {
-#if 0
-    osi_Log0(smb_logp,"ReceiveTran2SetPathInfo - NOT_SUPPORTED");
-    return CM_ERROR_BADOP;
-#else
     long code = 0;
     unsigned short infoLevel;
     clientchar_t * pathp;
@@ -3968,7 +3964,6 @@ long smb_ReceiveTran2SetPathInfo(smb_vc_t *vcp, smb_tr
     smb_FreeTran2Packet(outp);
 
     return 0;
-#endif
 }
 
 /* TRANS2_QUERY_FILE_INFORMATION */
