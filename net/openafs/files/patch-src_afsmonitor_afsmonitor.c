--- src/afsmonitor/afsmonitor.c.orig	2021-01-14 21:08:41 UTC
+++ src/afsmonitor/afsmonitor.c
@@ -56,7 +56,6 @@ int afsmon_debug = 0;		/* debug info to file ? */
 FILE *debugFD;			/* debugging file descriptor */
 static int afsmon_output = 0;	/* output to file ? */
 static int afsmon_detOutput = 0;	/* detailed output ? */
-static int afsmon_onceOnly = 0;	/* probe once only ? (not implemented) */
 int afsmon_probefreq;		/* probe frequency */
 static int wpkg_to_use;		/* graphics package to use */
 static char output_filename[80];	/* output filename */
@@ -3794,8 +3793,6 @@ afsmon_execute(void)
 	}
 
 	FSinitFlags = 0;
-	if (afsmon_onceOnly)	/* option not provided at this time */
-	    FSinitFlags |= XSTAT_FS_INITFLAG_ONE_SHOT;
 
 	if (afsmon_debug) {
 	    fprintf(debugFD, "[ %s ] Calling xstat_fs_Init \n", rn);
@@ -3867,8 +3864,6 @@ afsmon_execute(void)
 	collIDs[num_cm_collections++] = AFSCB_XSTATSCOLL_FULL_PERF_INFO;
 
 	CMinitFlags = 0;
-	if (afsmon_onceOnly)	/* once only ? */
-	    CMinitFlags |= XSTAT_CM_INITFLAG_ONE_SHOT;
 
 	if (afsmon_debug) {
 	    fprintf(debugFD, "[ %s ] Calling xstat_cm_Init \n", rn);
@@ -3892,19 +3887,6 @@ afsmon_execute(void)
 
 
     /* end of process cache manager entries */
-    /* if only one probe was required setup a waiting process for the
-     * termination signal */
-    if (afsmon_onceOnly) {
-	code = LWP_WaitProcess(&terminationEvent);
-	if (code) {
-	    if (afsmon_debug) {
-		fprintf(debugFD, "LWP_WaitProcess() returned error %d\n",
-			code);
-		fflush(debugFD);
-	    }
-	    afsmon_Exit(135);
-	}
-    }
 
     /* start the gtx input server */
     code = (intptr_t)gtx_InputServer(afsmon_win);
@@ -3990,25 +3972,6 @@ afsmonInit(struct cmd_syndesc *as, void *arock)
 
 
     /* use curses always until we support other packages */
-#ifdef notdef
-    wpkg_to_use = atoi(as->parms[P_PACKAGE].items->data);
-
-    switch (wpkg_to_use) {
-    case GATOR_WIN_CURSES:
-	fprintf(stderr, "curses\n");
-	break;
-    case GATOR_WIN_DUMB:
-	fprintf(stderr, "dumb terminal\n");
-	break;
-    case GATOR_WIN_X11:
-	fprintf(stderr, "X11\n");
-	break;
-    default:
-	fprintf(stderr, "Illegal graphics package: %d\n", wpkg_to_use);
-	afsmon_Exit(155);
-    }				/*end switch (wpkg_to_use) */
-#endif
-
     wpkg_to_use = GATOR_WIN_CURSES;
 
     /* get probe frequency . We check for meaningful bounds on the frequency
@@ -4263,11 +4226,6 @@ main(int argc, char **argv)
     cmd_AddParm(ts, "-output", CMD_SINGLE, CMD_OPTIONAL, "storage file name");
     cmd_AddParm(ts, "-detailed", CMD_FLAG, CMD_OPTIONAL,
 		"output detailed statistics to storage file");
-#ifdef notdef
-    /* we hope to use this .... eventually! */
-    cmd_AddParm(ts, "-package", CMD_SINGLE, CMD_REQUIRED,
-		"Graphics Package to use");
-#endif
     cmd_AddParm(ts, "-debug", CMD_SINGLE, CMD_OPTIONAL,
 		"turn debugging output on to the named file");
     cmd_AddParm(ts, "-fshosts", CMD_LIST, CMD_OPTIONAL,
