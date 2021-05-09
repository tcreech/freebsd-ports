--- src/ptserver/ptprocs.c.orig	2021-01-14 21:08:41 UTC
+++ src/ptserver/ptprocs.c
@@ -438,15 +438,6 @@ dumpEntry(struct rx_call *call, afs_int32 apos, struct
     if (!AccessOK(tt, *cid, 0, PRP_STATUS_MEM, 0))
 	ABORT_WITH(tt, PRPERM);
 
-    /* Since prdebugentry is in the form of a prentry not a coentry, we will
-     * return the coentry slots in network order where the string is. */
-#if 0
-    if (aentry->flags & PRCONT) {	/* wrong type, get coentry instead */
-	code = pr_ReadCoEntry(tt, 0, apos, aentry);
-	if (code)
-	    ABORT_WITH(tt, code);
-    }
-#endif
     code = ubik_EndTrans(tt);
     if (code)
 	return code;
@@ -1794,10 +1785,6 @@ listSuperGroups(struct rx_call *call, afs_int32 aid, p
 	ABORT_WITH(tt, PRPERM);
     if (!pr_noAuth && restrict_anonymous && *cid == ANONYMOUSID)
 	ABORT_WITH(tt, PRPERM);
-
-    code = ubik_SetLock(tt, 1, 1, LOCKREAD);
-    if (code)
-	ABORT_WITH(tt, code);
 
     temp = FindByID(tt, aid);
     if (!temp)
