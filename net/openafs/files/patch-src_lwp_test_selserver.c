--- src/lwp/test/selserver.c.orig	2021-01-14 21:08:41 UTC
+++ src/lwp/test/selserver.c
@@ -322,13 +322,6 @@ handleRequest(char *arg)
 	    Log("Probed from client at %s\n",
 		inet_ntoa(ch->ch_addr.sin_addr));
 	    break;
-#ifdef notdef
-	case SC_OOB:
-	    nThreads--;
-	    ch->ch_fd = 0;
-	    ch->ch_state = CH_FREE;
-	    return;
-#endif
 	case SC_WRITE:
 	    handleWrite(ch, &sc);
 	    break;
