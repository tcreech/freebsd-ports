--- src/bucoord/dump.c.orig	2021-01-14 21:08:41 UTC
+++ src/bucoord/dump.c
@@ -308,33 +308,6 @@ bc_StartDmpRst(struct bc_config *aconfig, char *adname
     return 0;
 }
 
-#ifdef notdef
-/* bc_FindDumpSlot
- * 	Returns the dump slot of the dump with dumpID
- * entry:
- *	dumpID - id to look for
- *	port - portoffset for tape coordinator
- * exit:
- *	0-n - i.e. 0 or positive number, is the dump slot
- *	-1 - failed to find dumpID
- */
-
-afs_int32
-bc_FindDumpSlot(afs_int32 dumpID, afs_int32 port)
-{
-    int i;
-
-    for (i = 0; i < BC_MAXSIMDUMPS; i++) {
-	if ((bc_dumpTasks[i].flags & BC_DI_INUSE)
-	    && (bc_dumpTasks[i].dumpID == dumpID)
-	    && ((afs_int32) bc_dumpTasks[i].portOffset == port)) {
-	    return (i);
-	}
-    }
-    return (-1);
-}
-#endif
-
 /* bc_LabelTape
  *	opens a connection to the tape coordinator and requests that it
  *	label a tape
