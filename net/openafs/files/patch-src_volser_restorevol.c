--- src/volser/restorevol.c.orig	2021-01-14 21:08:41 UTC
+++ src/volser/restorevol.c
@@ -362,19 +362,6 @@ struct vNode {
     afs_int32 modebits;
     afs_int32 parent;
     char acl[192];
-#ifdef notdef
-    struct acl_accessList {
-	int size;		/*size of this access list in bytes, including MySize itself */
-	int version;		/*to deal with upward compatibility ; <= ACL_ACLVERSION */
-	int total;
-	int positive;		/* number of positive entries */
-	int negative;		/* number of minus entries */
-	struct acl_accessEntry {
-	    int id;		/*internally-used ID of user or group */
-	    int rights;		/*mask */
-	} entries[100];
-    } acl;
-#endif
     afs_sfsize_t dataSize;
 };
 
@@ -923,10 +910,6 @@ WorkerBee(struct cmd_syndesc *as, void *arock)
     char *name = NULL;
     char *thisdir, *t;
     struct DumpHeader dh;	/* Defined in dump.h */
-#if 0/*ndef HAVE_GETCWD*/	/* XXX enable when autoconf happens */
-    extern char *getwd();
-#define getcwd(x,y) getwd(x)
-#endif
     thisdir = calloc(1, MAXPATHLEN+4);
     if (!thisdir)
 	goto mem_error_exit;
