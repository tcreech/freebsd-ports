--- src/gtx/curses_test.c.orig	2021-01-14 21:08:41 UTC
+++ src/gtx/curses_test.c
@@ -51,14 +51,6 @@ main(argc, argv)
     refresh();
     standend();
 
-#if 0
-    box addstr("Enter a string and a number: ");
-    refresh();
-    scanw(stdscr, "%s %d", str, &i);
-    wprintw(stdscr, "String was '%s', number was %d\n", str, i);
-    refresh();
-#endif /* 0 */
-
     endwin();
 
 }				/*main */
