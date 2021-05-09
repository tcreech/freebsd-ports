--- src/afsmonitor/afsmon-win.c.orig	2021-01-14 21:08:41 UTC
+++ src/afsmonitor/afsmon-win.c
@@ -27,10 +27,6 @@
 #include <afs/gtxwindows.h>		/*Generic window package */
 #include <afs/gtxobjects.h>		/*Object definitions */
 #include <afs/gtxkeymap.h>
-#if 0
-#include <afs/gtxtextcb.h>		/*Text object circular buffer interface */
-#include <afs/gtxtextobj.h>		/*Text object interface */
-#endif
 #include <afs/gtxlightobj.h>	/*Light object interface */
 #include <afs/gtxcurseswin.h>	/*Curses window package */
 #include <afs/gtxdumbwin.h>		/*Dumb terminal window package */
