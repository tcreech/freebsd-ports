--- src/rx/AIX/rx_knet.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/AIX/rx_knet.c
@@ -200,21 +200,6 @@ rxk_RX_input(struct mbuf *am)
 	    m_freem(am);
 	    return;
 	}
-#else
-#ifdef notdef
-	{			/* in_cksum() doesn't work correctly or the length is wrong? */
-	    int cksum;
-	    int glockOwner = ISAFS_GLOCK();
-	    cksum = in_cksum(am, sizeof(struct ip) + tlen);
-	    if (!glockOwner)
-		AFS_GLOCK();
-	    afs_Trace3(afs_iclSetp, CM_TRACE_WASHERE, ICL_TYPE_STRING,
-		       __FILE__, ICL_TYPE_INT32, __LINE__, ICL_TYPE_INT32,
-		       cksum);
-	    if (!glockOwner)
-		AFS_GUNLOCK();
-	}
-#endif
 #endif
     }
 
