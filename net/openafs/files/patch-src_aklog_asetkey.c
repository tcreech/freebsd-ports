--- src/aklog/asetkey.c.orig	2021-01-14 21:08:41 UTC
+++ src/aklog/asetkey.c
@@ -239,20 +239,54 @@ addKey(struct afsconf_dir *dir, int argc, char **argv)
 static void
 deleteKey(struct afsconf_dir *dir, int argc, char **argv)
 {
+    int type;
+    int subtype;
     int kvno;
     int code;
 
-    if (argc != 3) {
+    switch (argc) {
+    case 3:
+	kvno = atoi(argv[2]);
+	code = afsconf_DeleteKey(dir, kvno);
+	if (code) {
+	    afs_com_err(argv[0], code, "while deleting key %d", kvno);
+	    exit(1);
+	}
+	printf("Deleted rxkad key %d\n", kvno);
+	break;
+
+    case 4:
+	type = stringToType(argv[2]);
+	kvno = atoi(argv[3]);
+	code = afsconf_DeleteKeyByType(dir, type, kvno);
+	if (code) {
+	    afs_com_err(argv[0], code, "while deleting key (type %d kvno %d)",
+			type, kvno);
+	    exit(1);
+	}
+	printf("Deleted key (type %d kvno %d)\n", type, kvno);
+	break;
+
+    case 5:
+	type = stringToType(argv[2]);
+	kvno = atoi(argv[3]);
+	subtype = atoi(argv[4]);
+	code = afsconf_DeleteKeyBySubType(dir, type, kvno, subtype);
+	if (code) {
+	    afs_com_err(argv[0], code, "while deleting key (type %d kvno %d subtype %d)\n",
+			type, kvno, subtype);
+	    exit(1);
+	}
+	printf("Deleted key (type %d kvno %d subtype %d)\n", type, kvno, subtype);
+	break;
+
+    default:
 	fprintf(stderr, "%s delete: usage is '%s delete <kvno>\n",
 		argv[0], argv[0]);
+	fprintf(stderr, "\tOR\n\t%s delete <type> <kvno>\n", argv[0]);
+	fprintf(stderr, "\tOR\n\t%s delete <type> <kvno> <subtype>\n", argv[0]);
 	exit(1);
     }
-    kvno = atoi(argv[2]);
-    code = afsconf_DeleteKey(dir, kvno);
-    if (code) {
-	afs_com_err(argv[0], code, "while deleting key %d", kvno);
-	exit(1);
-    }
 }
 
 static void
@@ -316,6 +350,8 @@ main(int argc, char *argv[])
 	        argv[0]);
 	fprintf(stderr, "\t\tEx: %s add 0 \"80b6a7cd7a9dadb6\"\n", argv[0]);
 	fprintf(stderr, "\t%s delete <kvno>\n", argv[0]);
+	fprintf(stderr, "\t%s delete <type> <kvno>\n", argv[0]);
+	fprintf(stderr, "\t%s delete <type> <kvno> <subtype>\n", argv[0]);
 	fprintf(stderr, "\t%s list\n", argv[0]);
 	exit(1);
     }
