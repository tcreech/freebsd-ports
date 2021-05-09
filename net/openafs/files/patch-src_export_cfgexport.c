--- src/export/cfgexport.c.orig	2021-01-14 21:08:41 UTC
+++ src/export/cfgexport.c
@@ -420,16 +420,6 @@ xlate_xtok(xp, kp, strp, szp)
 	while (len >= left) {
 	    fprintf(stderr, "cfgexport: Out of memory. Increase SYMBUFSIZE and recompile\n");
 	    exit(1);
-#if 0
-	    /* Something is broken with this code, after being here
-	       cfgexport segfaults */
-	    export_strings = (char *)realloc(*strp, sz += SYMBUFSIZE);
-	    if (!export_strings)
-		error("no memory for EXPORT string table");
-	    *strp = export_strings;
-	    left += SYMBUFSIZE;
-	    prev = "";		/* lazy */
-#endif
 	}
 
 	strcpy(prev = *strp + offset, xstrings + xp->n_offset);
