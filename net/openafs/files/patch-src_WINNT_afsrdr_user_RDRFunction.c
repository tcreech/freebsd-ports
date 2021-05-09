--- src/WINNT/afsrdr/user/RDRFunction.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsrdr/user/RDRFunction.c
@@ -4091,24 +4091,6 @@ RDR_RequestFileExtentsAsync( IN cm_user_t *userp,
                 bHaveBuffer = TRUE;
             } else if (bufp->flags & CM_BUF_DIRTY) {
                 bHaveBuffer = FALSE;
-#if 0
-                code = buf_CleanAsyncLocked(scp, bufp, &req, 0, NULL);
-                switch (code) {
-                case 0:
-                    bHaveBuffer = TRUE;
-                    break;
-                case CM_ERROR_RETRY:
-                    /* Couldn't flush it, obtain it asynchronously so we don't block the thread. */
-                    bHaveBuffer = FALSE;
-                    code = 0;
-                    break;
-                default:
-                    smb_MapNTError(cm_MapRPCError(code, &req), &status, TRUE);
-                    RDR_SetFileStatus(&FileId, &userp->authgroup, status);
-                    bHaveBuffer = FALSE;
-                    code = 0;
-                }
-#endif
             } else {
                 osi_hyper_t minLength;  /* effective end of file */
 
