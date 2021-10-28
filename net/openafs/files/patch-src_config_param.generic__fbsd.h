--- src/config/param.generic_fbsd.h.orig	2021-07-29 10:24:31 UTC
+++ src/config/param.generic_fbsd.h
@@ -53,6 +53,7 @@
 #endif
 
 #define RXK_LISTENER_ENV	1
+#define AFS_PAG_ONEGROUP_ENV	1	/* Use a single gid to indicate a PAG */
 #define AFS_GCPAGS		0	/* if nonzero, garbage collect PAGs */
 #define AFS_USE_GETTIMEOFDAY	1	/* use gettimeofday to implement rx clock */
 
@@ -121,9 +122,9 @@ enum vcexcl { NONEXCL, EXCL };
 #define FBSD_IFA_IFWITHNET_THREE_ARGS
 #endif
 
-/* r273707 added a flags argument to syscall_register() */
+/* r273707 added a flags argument to syscall_register/syscall_helper_register */
 #if __FreeBSD_version >= 1100041
-#define FBSD_SYSCALL_REGISTER_FOUR_ARGS
+#define FBSD_SYSCALL_REGISTER_TAKES_FLAGS
 #endif
 
 /* r285819 eliminated b_saveaddr from struct buf */
@@ -134,6 +135,52 @@ enum vcexcl { NONEXCL, EXCL };
 /* r292373 changed the KPI for VOP_GETPAGES */
 #if __FreeBSD_version >= 1100092
 #define FBSD_VOP_GETPAGES_BUSIED
+#endif
+
+/* r333813 changed network interfaces and addrs to be traversed via
+ * CK_STAILQ_FOREACH instead of TAILQ_FOREACH */
+#if __FreeBSD_version >= 1200064
+# define AFS_FBSD_NET_FOREACH CK_STAILQ_FOREACH
+#else
+# define AFS_FBSD_NET_FOREACH TAILQ_FOREACH
+#endif
+
+/* b249ce48ea5 removed the flags argument from VOP_UNLOCK */
+#if __FreeBSD_version >= 1300074
+# define AFS_FBSD_VOP_UNLOCK_NOFLAGS
+#endif
+
+/* a68cc388790 requires explicit epoch_tracker declarations */
+#if __FreeBSD_version >= 1300008
+# define AFS_FBSD_EXPLICIT_EPOCH_TRACKER
+#endif
+
+/* a92a971bbb9 removed the thread argument from vget */
+#if __FreeBSD_version >= 1300109
+# define AFS_FBSD_VGET_NOTHREAD
+#endif
+
+/* abd80ddb947 removed the VI_DOOMED flag, introducing VN_IS_DOOMED() */ 
+# if __FreeBSD_version >= 1300063
+/* VN_IS_DOOMED() will be defined by <sys/vnode.h> */
+# else
+/* For older FreeBSD, define an equivalent VN_IS_DOOMED() macro */
+#  define VN_IS_DOOMED(vp) ((vp)->v_iflag & VI_DOOMED)
+# endif
+
+/* 756a5412798 removed getpbuf in favor of UMA */
+#if __FreeBSD_version >= 1300008
+# define AFS_FBSD_UMA_BUFS
+#endif
+
+/* 67d0e293048 removed OBJ_MIGHTBEDIRTY in favor of vm_object_mightbedirty */
+#if __FreeBSD_version >= 1300054
+# define AFS_FBSD_MIGHTBEDIRTY_HELPER
+#endif
+
+/* 6fa079fc3f5 requires explicit registration of the vector of vnode ops */
+#if __FreeBSD_version >= 1300067
+# define AFS_FBSD_HAS_VOP_VECTOR_REGISTER
 #endif
 
 #else /* !defined(UKERNEL) */
