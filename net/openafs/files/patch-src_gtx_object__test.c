--- src/gtx/object_test.c.orig	2021-01-14 21:08:41 UTC
+++ src/gtx/object_test.c
@@ -66,12 +66,6 @@ test_objects(pkg)
     int code;		/*Return code */
     struct onode_initparams oi_params;	/*Init params */
     struct gwin_initparams wi_params;	/*Window initialization params */
-#if 0
-    /*We don't need these, do we? */
-    struct gator_cursesgwin_params c_crparams;	/*Curses window creation params */
-    struct gator_dumbgwin_params d_crparams;	/*Dumb terminal window creation params */
-    struct gator_X11gwin_params x_crparams;	/*X11 window creation params */
-#endif /* 0 */
     struct gator_light_crparams light_crparams;	/*Light creation params */
     char helpstring1[128];	/*Help string to use */
     char helpstring2[128];	/*Help string to use */
