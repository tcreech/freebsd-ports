--- src/ptserver/ptclient.c.orig	2021-01-14 21:08:41 UTC
+++ src/ptserver/ptclient.c
@@ -305,16 +305,6 @@ main(int argc, char **argv)
 		printf("%s\n", pr_ErrorMsg(code));
 	    if (code == PRSUCCESS) {
 		PrintEntry(pos, &entry, /*indent */ 0);
-#if 0
-		printf("The contents of the entry for %d are:\n", entry.id);
-		printf("flags %d next %d\n", entry.flags, entry.next);
-		printf("Groups (or members) \n");
-		for (i = 0; i < PRSIZE; i++)
-		    printf("%d\n", entry.entries[i]);
-		printf("nextID %d nextname %d name %s\n", entry.nextID,
-		       entry.nextName, entry.name);
-		printf("owner %d creator %d\n", entry.owner, entry.creator);
-#endif
 	    }
 	} else if (!strcmp(op, "add") || !strcmp(op, "au")) {
 	    /* scanf("%d %d",&id,&gid); */
