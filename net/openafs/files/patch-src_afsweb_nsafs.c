--- src/afsweb/nsafs.c.orig	2021-01-14 21:08:41 UTC
+++ src/afsweb/nsafs.c
@@ -916,13 +916,7 @@ nsafs_basic(pblock * pb, Session * sn, Request * rq)
     code = uafs_klog(user, cell, passwd, &reason);
     memset((void *)&passwd[0], 0, NSAFS_PASSWORD_MAX);
     if (code != 0) {
-#if 0
-	sprintf(txtbuf, "%s@%s: %s\n", user, cell, reason);
-	pblock_nvinsert("status", "Login Failed", rq->vars);
-	return nsafs_error_check(EPERM, txtbuf, pb, sn, rq);
-#else /* 0 */
 	return REQ_PROCEED;
-#endif /* 0 */
     }
     expiration = u.u_expiration;
     usr_assert(expiration != 0);
