--- src/afs/FBSD/osi_machdep.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/FBSD/osi_machdep.h
@@ -22,6 +22,7 @@
 #include <sys/lock.h>
 #include <sys/time.h>
 #include <sys/mutex.h>
+#include <sys/sx.h>
 #include <sys/vnode.h>
 #include <sys/priv.h>
 
@@ -41,8 +42,6 @@ typedef struct proc afs_proc_t;
 
 #define VSUID           S_ISUID
 #define VSGID           S_ISGID
-
-#define osi_vnhold(avc,r)	vref(AFSTOV(avc))
 
 #define vType(vc)               AFSTOV(vc)->v_type
 #define vSetVfsp(vc, vfsp) 	AFSTOV(vc)->v_mount = (vfsp)
