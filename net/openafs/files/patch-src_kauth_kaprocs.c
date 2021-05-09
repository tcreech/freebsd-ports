--- src/kauth/kaprocs.c.orig	2021-01-14 21:08:41 UTC
+++ src/kauth/kaprocs.c
@@ -1144,15 +1144,6 @@ Authenticate(int version, struct rx_call *call, char *
 #endif /* EXPIREPW */
 
     if (check_ka_skew(request.time, now, KTC_TIME_UNCERTAINTY)) {
-#if 0
-	if (oanswer->MaxSeqLen < sizeof(afs_int32))
-	    code = KAANSWERTOOLONG;
-	else {			/* return our time if possible */
-	    oanswer->SeqLen = sizeof(afs_int32);
-	    request.time = htonl(now);
-	    memcpy(oanswer->SeqBody, &request.time, sizeof(afs_int32));
-	}
-#endif
 	code = KACLOCKSKEW;
 	goto abort;
     }
