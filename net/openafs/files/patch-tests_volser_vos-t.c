--- tests/volser/vos-t.c.orig	2021-01-14 21:08:41 UTC
+++ tests/volser/vos-t.c
@@ -78,7 +78,7 @@ TestListAddrs(struct ubik_client *client, char *dirnam
 	    exit(1);
 	}
 	execl(binPath, "vos",
-	      "listaddrs", "-config", dirname, "-noauth", NULL);
+	      "listaddrs", "-config", dirname, "-noauth", "-noresolve", NULL);
 	exit(1);
     }
     close(outpipe[1]);
@@ -100,6 +100,7 @@ main(int argc, char **argv)
     struct rx_securityClass *secClass;
     struct ubik_client *ubikClient = NULL;
     int ret = 0;
+    char *argv0 = afstest_GetProgname(argv);
 
     /* Skip all tests if the current hostname can't be resolved */
     afstest_SkipTestsIfBadHostname();
@@ -116,14 +117,14 @@ main(int argc, char **argv)
 
     code = afstest_AddDESKeyFile(dir);
     if (code) {
-	afs_com_err("vos-t", code, "while adding test DES keyfile");
+	afs_com_err(argv0, code, "while adding test DES keyfile");
 	ret = 1;
 	goto out;
     }
 
     code = afstest_StartVLServer(dirname, &serverPid);
     if (code) {
-	afs_com_err("vos-t", code, "while starting the vlserver");
+	afs_com_err(argv0, code, "while starting the vlserver");
 	ret = 1;
 	goto out;
     }
@@ -133,7 +134,7 @@ main(int argc, char **argv)
     code = afsconf_ClientAuthSecure(dir, &secClass, &secIndex);
     is_int(code, 0, "Successfully got security class");
     if (code) {
-	afs_com_err("authname-t", code, "while getting anonymous secClass");
+	afs_com_err(argv0, code, "while getting anonymous secClass");
 	ret = 1;
 	goto out;
     }
@@ -142,7 +143,7 @@ main(int argc, char **argv)
 				 secClass, secIndex, &ubikClient);
     is_int(code, 0, "Successfully built ubik client structure");
     if (code) {
-	afs_com_err("vos-t", code, "while building ubik client");
+	afs_com_err(argv0, code, "while building ubik client");
 	ret = 1;
 	goto out;
     }
