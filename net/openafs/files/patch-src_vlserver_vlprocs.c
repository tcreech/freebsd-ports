--- src/vlserver/vlprocs.c.orig	2021-01-14 21:08:41 UTC
+++ src/vlserver/vlprocs.c
@@ -1841,7 +1841,7 @@ ListAttributesN2(struct rx_call *rxcall,
 		 * pick up entries marked NEWREPSITEs and DONTUSE.
 		 */
 		else {
-		    if (tentry.flags & VLF_ROEXISTS) {
+		    if ((tentry.serverFlags[k] & VLSF_ROVOL) != 0) {
 			if (findname) {
 			    if (namematchRO) {
 				thismatch =
@@ -3054,7 +3054,7 @@ vldbentry_to_vlentry(struct vl_ctx *ctx,
 	VlEntry->serverPartition[i] = VldbEntry->serverPartition[i];
 	VlEntry->serverFlags[i] = VldbEntry->serverFlags[i];
     }
-    for (; i < OMAXNSERVERS; i++)
+    for (; i < NMAXNSERVERS; i++)
 	VlEntry->serverNumber[i] = VlEntry->serverPartition[i] =
 	    VlEntry->serverFlags[i] = BADSERVERID;
     for (i = 0; i < MAXTYPES; i++)
