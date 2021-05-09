--- src/gtx/gtxX11win.h.orig	2021-01-14 21:08:41 UTC
+++ src/gtx/gtxX11win.h
@@ -15,17 +15,6 @@
 /*Value for gwin w_type field*/
 #define	GATOR_WIN_X11    3
 
-/*Private data for a X11 gwin*/
-#if 0
-struct gator_X11gwin {
-    WINDOW *wp;			/*Window pointer */
-    int charwidth;		/*Character width in pixels */
-    int charheight;		/*Character height in pixels */
-    char box_vertchar;		/*Vertical char for boxing purposes */
-    char box_horizchar;		/*Horizontal char for boxing purposes */
-};
-#endif /* 0 */
-
 /*X11 gwin's creation parameters*/
 struct gator_X11gwin_params {
     struct gwin_createparams gwin_params;	/*Basic params for the window */
