--- src/afs/LINUX/osi_probe.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_probe.c
@@ -427,12 +427,6 @@ static int main_zapped_syscalls[] = {
     1132, 1136, 1135,
 #endif
 
-/* And the same deal for arm (not arm26), if we ever support that. */
-#if 0
-    /* create_module, query_module, get_kernel_sysms */
-    127, 167, 130,
-#endif
-
 /*
  * Alpha-Linux uses syscall number mappings chosen to be compatible
  * with OSF/1.  So, it doesn't have any of the traditional calls or
