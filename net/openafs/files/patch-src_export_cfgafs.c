--- src/export/cfgafs.c.orig	2021-01-14 21:08:41 UTC
+++ src/export/cfgafs.c
@@ -111,9 +111,6 @@ main(argc, argv)
 	    sysconfig(SYS_KULOAD, &cload, sizeof(cload));
 	    exit(1);
 	}
-#ifdef notdef
-	printf("cfgafs -d 0x%x # to remove AFS\n", cload.kmid);
-#endif
 	strcpy(PidFile, file);
 	strcat(PidFile, ".kmid");
 	fp = fopen(PidFile, "w");
