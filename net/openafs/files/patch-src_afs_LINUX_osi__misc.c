--- src/afs/LINUX/osi_misc.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/LINUX/osi_misc.c
@@ -155,10 +155,21 @@ int osi_abspath(char *aname, char *buf, int buflen,
 
 
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
@@ -166,7 +177,12 @@ int afs_thread_wrapper(void *rock)
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
