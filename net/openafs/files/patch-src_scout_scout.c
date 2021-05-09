--- src/scout/scout.c.orig	2021-01-14 21:08:41 UTC
+++ src/scout/scout.c
@@ -35,9 +35,6 @@
  */
 #define P_SERVER    0
 #define P_BASE	    1
-#if 0
-#define	P_PACKAGE   2
-#endif /* 0 */
 #define P_FREQ	    2
 #define P_HOST	    3
 #define P_ATTENTION 4
@@ -1663,9 +1660,6 @@ execute_scout(int a_numservers, struct cmd_item *a_srv
     /*
      * Initialize the gtx package.
      */
-#if 0
-    fprintf(stderr, "[%s:%s] Starting up gtx package\n", pn, rn);
-#endif /* 0 */
     scout_gwin = gtx_Init(0,	/*Don't start up InputServer yet */
 			  -1);	/*Type of window package */
     if (scout_gwin == NULL) {
@@ -1907,22 +1901,7 @@ execute_scout(int a_numservers, struct cmd_item *a_srv
 
     }				/*Add lights in server records to display list */
 
-#if 0
     /*
-     * Set up the minimal keymap.
-     */
-    code = keymap_BindToString(scout_frame->keymap,	/*Ptr to keymap */
-			       "e",	/*Key to bind */
-			       ExitCmd,	/*Cmd to execute */
-			       NULL,	/*Name */
-			       NULL);	/*Ptr to rock */
-    if (code) {
-	fprintf(stderr, "[%s] Can't bind key `e', code is %d\n", rn, code);
-	return (code);
-    }
-#endif /* 0 */
-
-    /*
      * Finish setting up the overall mini_screen structure.
      */
     scout_screen.numServers = a_numservers;
@@ -1940,9 +1919,6 @@ execute_scout(int a_numservers, struct cmd_item *a_srv
 			scout_probefreq,	/*Probe frequency */
 			FS_Handler,	/*Handler routine */
 			0);	/*Turn debugging output off */
-#if 0
-    scout_debug);		/*Turn debugging output off */
-#endif /* 0 */
     if (code) {
 	fprintf(stderr, "[%s] Error returned by fsprobe_Init: %d\n", rn,
 		code);
@@ -2248,29 +2224,7 @@ static int scoutInit(struct cmd_syndesc *as, void *aro
 	fprintf(scout_debugfd, "[%s] Writing to Scout debugging file '%s'\n",
 		rn, debug_filename);
     }
-#if 0
-    wpkg_to_use = atoi(as->parms[P_PACKAGE].items->data);
-#endif /* 0 */
     wpkg_to_use = 2;		/*Always use curses for now */
-#if 0
-    fprintf(stderr, "[%s:%s] Using graphics package %d: ", pn, rn,
-	    wpkg_to_use);
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
-	scout_CleanExit(-1);
-    }				/*end switch (wpkg_to_use) */
-#endif /* 0 */
-
     if (as->parms[P_FREQ].items != 0)
 	scout_probefreq = atoi(as->parms[P_FREQ].items->data);
     else
@@ -2365,10 +2319,6 @@ main(int argc, char **argv)
 		"FileServer name(s) to monitor");
     cmd_AddParm(ts, "-basename", CMD_SINGLE, CMD_OPTIONAL,
 		"base server name");
-#if 0
-    cmd_AddParm(ts, "-package", CMD_SINGLE, CMD_REQUIRED,
-		"Graphics package to use");
-#endif /* 0 */
     cmd_AddParm(ts, "-frequency", CMD_SINGLE, CMD_OPTIONAL,
 		"poll frequency, in seconds");
     cmd_AddParm(ts, "-host", CMD_FLAG, CMD_OPTIONAL,
@@ -2386,10 +2336,6 @@ main(int argc, char **argv)
      */
     code = cmd_Dispatch(argc, argv);
     if (code) {
-#if 0
-	fprintf(stderr, "[%s:%s] Call to cmd_Dispatch() failed; code is %d\n",
-		pn, rn, code);
-#endif /* 0 */
 	scout_CleanExit(1);
     } else
 	scout_CleanExit(0);
