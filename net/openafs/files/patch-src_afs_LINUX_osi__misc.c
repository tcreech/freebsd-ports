--- src/afs/LINUX/osi_misc.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_misc.c
@@ -138,13 +138,7 @@ int osi_abspath(char *aname, char *buf, int buflen,
 	return -PTR_ERR(name);
     code = osi_lookupname_internal(name, followlink, &mnt, &dp);
     if (!code) {
-#if defined(D_PATH_TAKES_STRUCT_PATH)
-	afs_linux_path_t p = { .mnt = mnt, .dentry = dp };
-	path = d_path(&p, buf, buflen);
-#else
-	path = d_path(dp, mnt, buf, buflen);
-#endif
-
+	path = afs_d_path(dp, mnt, buf, buflen);
 	if (IS_ERR(path)) {
 	    code = -PTR_ERR(path);
 	} else {
@@ -161,10 +155,21 @@ int osi_abspath(char *aname, char *buf, int buflen,
 
 
 /* This could use some work, and support on more platforms. */
-int afs_thread_wrapper(void *rock)
+static int
+afs_thread_wrapper(void *rock)
 {
     void (*proc)(void) = rock;
     __module_get(THIS_MODULE);
+    (*proc)();
+    module_put(THIS_MODULE);
+    return 0;
+}
+
+static int
+afs_thread_wrapper_glock(void *rock)
+{
+    void (*proc)(void) = rock;
+    __module_get(THIS_MODULE);
     AFS_GLOCK();
     (*proc)();
     AFS_GUNLOCK();
@@ -172,7 +177,12 @@ int afs_thread_wrapper(void *rock)
     return 0;
 }
 
-void afs_start_thread(void (*proc)(void), char *name)
+void
+afs_start_thread(void (*proc)(void), char *name, int needs_glock)
 {
-    kthread_run(afs_thread_wrapper, proc, "%s", name);
+    if (needs_glock) {
+	kthread_run(afs_thread_wrapper_glock, proc, "%s", name);
+    } else {
+	kthread_run(afs_thread_wrapper, proc, "%s", name);
+    }
 }
