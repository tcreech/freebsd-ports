--- src/rx/NBSD/rx_knet.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/NBSD/rx_knet.c
@@ -39,9 +39,6 @@ osi_NetReceive(osi_socket asocket, struct sockaddr_in 
     u.uio_resid = *alength;
     UIO_SETUP_SYSSPACE(&u);
     u.uio_rw = UIO_READ;
-#if 0
-    u.uio_procp = NULL;
-#endif
     if (glocked)
 	AFS_GUNLOCK();
     code = soreceive(asocket, (addr ? &nam : NULL), &u, NULL, NULL, NULL);
@@ -113,9 +110,6 @@ osi_NetSend(osi_socket asocket, struct sockaddr_in *ad
     u.uio_resid = alength;
     UIO_SETUP_SYSSPACE(&u);
     u.uio_rw = UIO_WRITE;
-#if 0
-    u.uio_procp = NULL;
-#endif
     nam = m_get(M_DONTWAIT, MT_SONAME);
     if (!nam)
 	return ENOBUFS;
