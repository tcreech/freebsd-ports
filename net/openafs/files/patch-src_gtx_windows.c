--- src/gtx/windows.c.orig	2021-01-14 21:08:41 UTC
+++ src/gtx/windows.c
@@ -20,11 +20,6 @@
 
 #include <roken.h>
 
-/* On DUX "IN" is a variable in curses.h, so this can be a bit of a problem */
-#ifdef IN
-#undef IN
-#endif
-
 #include "gtxwindows.h"		/*Interface for this module */
 #include "gtxcurseswin.h"	/*Interface for the curses module */
 #include "gtxdumbwin.h"		/*Interface for the dumb terminal module */
