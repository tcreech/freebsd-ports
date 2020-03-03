--- src/afs/FBSD/osi_vnodeops.c.orig	2020-02-21 20:17:12 UTC
+++ src/afs/FBSD/osi_vnodeops.c
@@ -61,16 +61,19 @@
 #include <vm/vm_object.h>
 #include <vm/vm_pager.h>
 #include <vm/vnode_pager.h>
+#if defined(AFS_FBSD120_ENV)
+#include <sys/vmmeter.h>
+#endif
 extern int afs_pbuf_freecnt;
 
 #define GETNAME()       \
     struct componentname *cnp = ap->a_cnp; \
     char *name; \
-    MALLOC(name, char *, cnp->cn_namelen+1, M_TEMP, M_WAITOK); \
+    name =  malloc(cnp->cn_namelen+1, M_TEMP, M_WAITOK); \
     memcpy(name, cnp->cn_nameptr, cnp->cn_namelen); \
     name[cnp->cn_namelen] = '\0'
 
-#define DROPNAME() FREE(name, M_TEMP)
+#define DROPNAME() free(name, M_TEMP)
 
 /*
  * Here we define compatibility functions/macros for interfaces that
@@ -85,8 +88,13 @@ static __inline void ma_vm_page_unlock(vm_page_t m) { 
 #define MA_VOP_LOCK(vp, flags, p) (VOP_LOCK(vp, flags))
 #define MA_VOP_UNLOCK(vp, flags, p) (VOP_UNLOCK(vp, flags))
 
+#if defined(AFS_FBSD120_ENV)
+#define MA_PCPU_INC(c) VM_CNT_INC(c)
+#define	MA_PCPU_ADD(c, n) VM_CNT_ADD(c, n)
+#else
 #define MA_PCPU_INC(c) PCPU_INC(c)
 #define	MA_PCPU_ADD(c, n) PCPU_ADD(c, n)
+#endif /* AFS_FBSD120_ENV */
 
 #if __FreeBSD_version >= 1000030
 #define AFS_VM_OBJECT_WLOCK(o)	VM_OBJECT_WLOCK(o)
@@ -110,7 +118,7 @@ afs_vop_pathconf(struct vop_pathconf_args *ap)
 	error = 0;
 	switch (ap->a_name) {
 	case _PC_LINK_MAX:
-		*ap->a_retval = LINK_MAX;
+		*ap->a_retval = 32767;
 		break;
 	case _PC_NAME_MAX:
 		*ap->a_retval = NAME_MAX;
@@ -534,8 +542,8 @@ afs_vop_getpages(struct vop_getpages_args *ap)
 
     kva = (vm_offset_t) bp->b_data;
     pmap_qenter(kva, pages, npages);
-    MA_PCPU_INC(cnt.v_vnodein);
-    MA_PCPU_ADD(cnt.v_vnodepgsin, npages);
+    MA_PCPU_INC(v_vnodein);
+    MA_PCPU_ADD(v_vnodepgsin, npages);
 
 #ifdef FBSD_VOP_GETPAGES_BUSIED
     count = ctob(npages);
@@ -709,8 +717,8 @@ afs_vop_putpages(struct vop_putpages_args *ap)
 
     kva = (vm_offset_t) bp->b_data;
     pmap_qenter(kva, ap->a_m, npages);
-    MA_PCPU_INC(cnt.v_vnodeout);
-    MA_PCPU_ADD(cnt.v_vnodepgsout, ap->a_count);
+    MA_PCPU_INC(v_vnodeout);
+    MA_PCPU_ADD(v_vnodepgsout, ap->a_count);
 
     iov.iov_base = (caddr_t) kva;
     iov.iov_len = ap->a_count;
@@ -927,10 +935,10 @@ afs_vop_rename(ap)
     if ((error = ma_vn_lock(fvp, LK_EXCLUSIVE, p)) != 0)
 	goto abortit;
 
-    MALLOC(fname, char *, fcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
+    fname = malloc(fcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
     memcpy(fname, fcnp->cn_nameptr, fcnp->cn_namelen);
     fname[fcnp->cn_namelen] = '\0';
-    MALLOC(tname, char *, tcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
+    tname = malloc(tcnp->cn_namelen + 1, M_TEMP, M_WAITOK);
     memcpy(tname, tcnp->cn_nameptr, tcnp->cn_namelen);
     tname[tcnp->cn_namelen] = '\0';
 
@@ -941,8 +949,8 @@ afs_vop_rename(ap)
 	afs_rename(VTOAFS(fdvp), fname, VTOAFS(tdvp), tname, tcnp->cn_cred);
     AFS_GUNLOCK();
 
-    FREE(fname, M_TEMP);
-    FREE(tname, M_TEMP);
+    free(fname, M_TEMP);
+    free(tname, M_TEMP);
     if (tdvp == tvp)
 	vrele(tdvp);
     else
@@ -1082,8 +1090,7 @@ afs_vop_readdir(ap)
 	     dp = (const struct dirent *)((const char *)dp + dp->d_reclen))
 	    ncookies++;
 
-	MALLOC(cookies, u_long *, ncookies * sizeof(u_long), M_TEMP,
-	       M_WAITOK);
+	cookies = malloc(ncookies * sizeof(u_long), M_TEMP, M_WAITOK);
 	for (dp = dp_start, cookiep = cookies; dp < dp_end;
 	     dp = (const struct dirent *)((const char *)dp + dp->d_reclen)) {
 	    off += dp->d_reclen;
