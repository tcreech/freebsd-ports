--- src/lwp/iomgr.c.orig	2021-01-14 21:08:41 UTC
+++ src/lwp/iomgr.c
@@ -527,7 +527,7 @@ static void *IOMGR(void *dummy)
 	       /* Tape drives on Sun boxes do not support select and return ENXIO */
 	       if (errno == ENXIO) e=0, code=1;
 #endif
-#if defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_OSF_ENV) || defined(AFS_AIX32_ENV)
+#if defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_AIX32_ENV)
 	       /* For SGI and SVR4 - poll & select can return EAGAIN ... */
 	       if (errno == EAGAIN) e=0;
 #endif
@@ -843,11 +843,11 @@ int IOMGR_Select(int fds, fd_set *readfds, fd_set *wri
 #ifdef DEBUG
 	    if (lwp_debug != 0) puts("[Polling SELECT]");
 #endif /* DEBUG */
-#if	defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_OSF_ENV) || defined(AFS_AIX32_ENV) || defined(AFS_NT40_ENV)
+#if	defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_AIX32_ENV) || defined(AFS_NT40_ENV)
 again:
 #endif
 	    code = select(fds, readfds, writefds, exceptfds, timeout);
-#if	defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_OSF_ENV) || defined(AFS_AIX32_ENV)
+#if	defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_AIX32_ENV)
 	    /*
 	     * For SGI and SVR4 - poll & select can return EAGAIN ...
 	     */
