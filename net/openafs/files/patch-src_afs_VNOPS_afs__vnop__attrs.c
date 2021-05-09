--- src/afs/VNOPS/afs_vnop_attrs.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/VNOPS/afs_vnop_attrs.c
@@ -248,7 +248,7 @@ afs_getattr(OSI_VC_DECL(avc), struct vattr *attrs, afs
 
     if (!(avc->f.states & CStatd)) {
 	if (!(code = afs_CreateReq(&treq, acred))) {
-	    code = afs_VerifyVCache2(avc, treq);
+	    code = afs_VerifyVCache(avc, treq);
 	    inited = 1;
 	}
     } else
@@ -411,11 +411,11 @@ afs_VAttrToAS(struct vcache *avc, struct vattr *av,
 #elif defined(AFS_HPUX_ENV)
 #if	defined(AFS_HPUX102_ENV)
     if (av->va_uid != UID_NO_CHANGE) {
-#elif	defined(AFS_XBSD_ENV)
-    if (av->va_uid != (uid_t)VNOVAL) {
 #else
     if (av->va_uid != ((unsigned short)-1)) {
 #endif
+#elif	defined(AFS_XBSD_ENV)
+    if (av->va_uid != (uid_t)VNOVAL) {
 #else
     if (av->va_uid != -1) {
 #endif /* AFS_LINUX22_ENV */
