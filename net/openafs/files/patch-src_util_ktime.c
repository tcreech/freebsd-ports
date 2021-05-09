--- src/util/ktime.c.orig	2021-01-14 21:08:41 UTC
+++ src/util/ktime.c
@@ -638,12 +638,6 @@ ktime_InterpretDate(struct ktime_date * akdate)
 	temp = tresult + tbit;	/* see if adding this bit keeps us < akdate */
 	tsp = localtime(&temp);
 	tsp->tm_mon++;
-#ifdef notdef
-	if (tsp->tm_mon == 0) {
-	    tsp->tm_mon = 12;
-	    tsp->tm_year--;
-	}
-#endif
 	if (KDateCmp(akdate, tsp) >= 0) {
 	    /* if temp still represents earlier than date than we're searching
 	     * for, add in bit as increment, otherwise use old value and look
