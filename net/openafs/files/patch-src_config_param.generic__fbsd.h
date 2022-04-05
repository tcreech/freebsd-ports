--- src/config/param.generic_fbsd.h.orig	2021-12-09 17:07:41 UTC
+++ src/config/param.generic_fbsd.h
@@ -53,6 +53,7 @@
 #endif
 
 #define RXK_LISTENER_ENV	1
+#define AFS_PAG_ONEGROUP_ENV	1	/* Use a single gid to indicate a PAG */
 #define AFS_GCPAGS		0	/* if nonzero, garbage collect PAGs */
 #define AFS_USE_GETTIMEOFDAY	1	/* use gettimeofday to implement rx clock */
 
@@ -142,6 +143,44 @@ enum vcexcl { NONEXCL, EXCL };
 # define AFS_FBSD_NET_FOREACH CK_STAILQ_FOREACH
 #else
 # define AFS_FBSD_NET_FOREACH TAILQ_FOREACH
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
