--- src/bucoord/dsstub.c.orig	2021-01-14 21:08:41 UTC
+++ src/bucoord/dsstub.c
@@ -174,21 +174,6 @@ static afs_int32 DeleteDump(afs_int32 adumpID)
     return code;
 }
 
-#if 0
-static afs_int32 DeleteTape(char * atapeName)
-{
-    char *tp;
-    afs_int32 code;
-
-    tp = TapeName(atapeName);
-    if (tp == NULL)
-	return ENOMEM;
-    code = unlink(tp);
-    free(tp);
-    return code;
-}
-#endif
-
 /* tailCompPtr
  *	name is a pathname style name, determine trailing name and return
  *	pointer to it
@@ -247,25 +232,6 @@ ScanDumpHdr(FILE *afile, char *aname, char *dumpName, 
     return 0;
 }
 
-#if 0
-/* scan a tape header out of a tape file, leaving the file ptr positioned just past the header */
-static afs_int32 ScanTapeHdr(FILE *afile, afs_int32 *adumpID, afs_int32 *aseq, afs_int32 *adamage)
-{
-    char tbuffer[256];
-    char *tp;
-    afs_int32 code;
-
-    tp = fgets(tbuffer, sizeof(tbuffer), afile);
-    if (!tp)
-	return -1;
-    code = sscanf(tbuffer, "%ld %ld %ld", (long int *)adumpID,
-		  (long int *)aseq, (long int *)adamage);
-    if (code != 3)
-	return -1;
-    return 0;
-}
-#endif
-
 /* ScanTapeVolume
  *	scan a tape volume record from a dump file, leaving the file ptr
  *	positioned past the just-scanned record.
@@ -323,23 +289,3 @@ afs_int32 ScanVolClone(FILE *tdump, char *volName, afs
     }
     return (-1);
 }
-
-#if 0
-/* seek a dump file (after a header scan has been done) to position apos */
-static int SeekDump(FILE *afile, afs_int32 apos)
-{
-    afs_int32 i;
-    char *tp;
-    char tbuffer[256];
-
-    /* now skip to appropriate position */
-    for (i = 0; i < apos; i++) {
-	tp = fgets(tbuffer, sizeof(tbuffer), afile);
-	if (!tp) {
-	    fclose(afile);
-	    return -1;
-	}
-    }
-    return 0;
-}
-#endif
