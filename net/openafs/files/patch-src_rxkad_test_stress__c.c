--- src/rxkad/test/stress_c.c.orig	2021-01-14 21:08:41 UTC
+++ src/rxkad/test/stress_c.c
@@ -419,25 +419,9 @@ RunLoadTest(struct clientParms *parms, struct rx_conne
 	    int b;
 	    kbps = (double)(parms->copiousCalls * n) / (interval * 1000.0);
 	    b = kbps + 0.5;
-#if 0
-	    I just cannot get printing of floats to work on the pmax !
-		!!!printf("%g %d %d %d\n", (float)kbps, b);
-	    printf("%g %d %d %d\n", kbps, b);
-	    fprintf(stdout, "%g %d %d\n", kbps, b);
-	    {
-		char buf[100];
-		buf[sizeof(buf) - 1] = 0;
-		sprintf(buf, "%g %d %d\n", kbps, b);
-		assert(buf[sizeof(buf) - 1] == 0);
-		printf("%s", buf);
-	    }
-#endif
 	    printf
 		("For %lu copious calls, %lu send + %lu recv = %lu bytes each: %d kbytes/sec\n",
 		 parms->copiousCalls, parms->sendLen, parms->recvLen, n, b);
-#if 0
-	    printf("%g\n", kbps);
-#endif
 	}
     }
     return 0;
@@ -1296,10 +1280,6 @@ rxkst_StartClient(struct clientParms *parms)
 
     if (parms->printStats) {
 	rx_PrintStats(stdout);
-#if 0
-	/* use rxdebug style iteration here */
-	rx_PrintPeerStats(stdout, rx_PeerOf(conn));
-#endif
     }
 
     rxs_Release(sc);
