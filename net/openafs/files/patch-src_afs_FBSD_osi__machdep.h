--- src/afs/FBSD/osi_machdep.h.orig	2021-01-14 21:08:41 UTC
+++ src/afs/FBSD/osi_machdep.h
@@ -22,10 +22,9 @@
 #include <sys/lock.h>
 #include <sys/time.h>
 #include <sys/mutex.h>
+#include <sys/sx.h>
 #include <sys/vnode.h>
-#if defined(AFS_FBSD80_ENV)
 #include <sys/priv.h>
-#endif
 
 /*
  * Time related macros
@@ -41,17 +40,13 @@ typedef struct proc afs_proc_t;
 #define iodone biodone
 #endif
 
-#ifdef AFS_FBSD80_ENV
 #define VSUID           S_ISUID
 #define VSGID           S_ISGID
-#endif
 
-#define osi_vnhold(avc,r)	vref(AFSTOV(avc))
-
 #define vType(vc)               AFSTOV(vc)->v_type
 #define vSetVfsp(vc, vfsp) 	AFSTOV(vc)->v_mount = (vfsp)
 #define vSetType(vc, type)      AFSTOV(vc)->v_type = (type)
-#if defined(AFS_FBSD60_ENV) && defined(KERNEL)
+#ifdef KERNEL
 extern struct vop_vector afs_vnodeops;
 # define IsAfsVnode(v) ((v)->v_op == &afs_vnodeops)
 #else
@@ -60,14 +55,8 @@ extern int (**afs_vnodeop_p) ();
 #endif
 #define SetAfsVnode(v)          /* nothing; done in getnewvnode() */
 
-#if defined(AFS_FBSD80_ENV)
 #define osi_vinvalbuf(vp, flags, slpflag, slptimeo) \
   vinvalbuf((vp), (flags), (slpflag), (slptimeo))
-#else
-#define osi_vinvalbuf(vp, flags, slpflag, slptimeo) \
-  vinvalbuf((vp), (flags), (curthread), (slpflag), (slptimeo))
-#define osi_curproc() (curthread)
-#endif
 
 #undef gop_lookupname
 #define gop_lookupname osi_lookupname
@@ -83,26 +72,18 @@ extern void osi_fbsd_free(void *p);
 
 #define afs_osi_Alloc_NoSleep(size) osi_fbsd_alloc((size), 0)
 
-#ifdef AFS_FBSD80_ENV
 #define VN_RELE(vp)				\
   do {						\
     vrele(vp);					\
   } while(0);
-#else
-#define VN_RELE(vp)             vrele(vp)
-#endif
 #define VN_HOLD(vp)		VREF(vp)
 
 #undef afs_suser
-#if defined(AFS_FBSD80_ENV)
 /* OpenAFS-specific privileges negotiated for FreeBSD, thanks due to
  * Ben Kaduk */
 #define osi_suser_client_settings(x)   (!priv_check(curthread, PRIV_AFS_ADMIN))
 #define osi_suser_afs_daemon(x)   (!priv_check(curthread, PRIV_AFS_DAEMON))
 #define afs_suser(x) (osi_suser_client_settings((x)) && osi_suser_afs_daemon((x)))
-#else
-#define afs_suser(x)	(!suser(curthread))
-#endif
 
 #undef osi_getpid
 #define VT_AFS		"afs"
@@ -129,7 +110,7 @@ extern struct thread *afs_global_owner;
 	mtx_unlock(&afs_global_mtx); \
     } while (0)
 #define ISAFS_GLOCK() (mtx_owned(&afs_global_mtx))
-# if defined(AFS_FBSD80_ENV) && defined(WITNESS)
+# ifdef WITNESS
 #  define osi_InitGlock() \
 	do { \
 	    memset(&afs_global_mtx, 0, sizeof(struct mtx)); \
