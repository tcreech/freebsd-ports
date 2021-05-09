--- src/afs/LINUX/osi_nfssrv.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_nfssrv.c
@@ -160,26 +160,6 @@ done:
 }
 
 
-#if 0
-/* This doesn't work, because they helpfully NULL out rqstp->authop
- * before calling us, so we have no way to tell what the original
- * auth flavor was.
- */
-static int
-svcauth_afs_release(struct svc_rqst *rqstp)
-{
-    struct nfs_server_thread *ns;
-
-    AFS_GLOCK();
-    ns = find_nfs_thread(0);
-    if (ns) ns->active = 0;
-    AFS_GUNLOCK();
-
-    return afs_orig_authtab[rqstp->rq_authop->flavour]->release(rqstp);
-}
-#endif
-
-
 int osi_linux_nfs_initreq(struct vrequest *av, afs_ucred_t *cr, int *code)
 {
     struct nfs_server_thread *ns;
