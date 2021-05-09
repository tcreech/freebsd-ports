--- src/tools/dumpscan/dumptool.c.orig	2021-01-14 21:08:41 UTC
+++ src/tools/dumpscan/dumptool.c
@@ -1028,9 +1028,6 @@ ScanVnodes(FILE * f, VolumeDiskData * vol, int sizesca
 		    fprintf(stderr, "failed readbystring for 'A'\n");
 		    return -1;
 		}
-#if 0
-		acl_NtohACL(VVnodeDiskACL(vnode));
-#endif
 		break;
 #ifdef RESIDENCY
 	    case 'h':
