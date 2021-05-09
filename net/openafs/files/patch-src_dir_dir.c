--- src/dir/dir.c.orig	2021-01-14 21:08:41 UTC
+++ src/dir/dir.c
@@ -38,11 +38,6 @@
 #   include "h/user.h"
 #  endif /* AFS_SGI64_ENV */
 #  include "h/uio.h"
-#  ifdef	AFS_OSF_ENV
-#   include <sys/mount.h>
-#   include <sys/vnode.h>
-#   include <ufs/inode.h>
-#  endif
 #  if !defined(AFS_SUN5_ENV) && !defined(AFS_LINUX20_ENV) && !defined(AFS_HPUX110_ENV)
 #   include "h/mbuf.h"
 #  endif
