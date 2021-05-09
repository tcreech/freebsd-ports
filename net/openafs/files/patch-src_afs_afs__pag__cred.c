--- src/afs/afs_pag_cred.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_pag_cred.c
@@ -231,9 +231,6 @@ SPAGCB_GetCreds(struct rx_call *a_call, afs_int32 a_ui
     memset(a_creds, 0, sizeof(struct CredInfos));
     if ((rx_HostOf(rx_PeerOf(rx_ConnectionOf(a_call))) != afs_nfs_server_addr
 	||  rx_PortOf(rx_PeerOf(rx_ConnectionOf(a_call))) != htons(7001))
-#if 0 /* for debugging ONLY! */
-	&&  rx_PortOf(rx_PeerOf(rx_ConnectionOf(a_call))) != htons(7901)
-#endif
         ) {
 	RX_AFS_GUNLOCK();
 	return UAEPERM;
