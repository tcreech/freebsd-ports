--- src/afs/DARWIN/osi_vfsops.c.orig	2021-07-29 10:24:31 UTC
+++ src/afs/DARWIN/osi_vfsops.c
@@ -286,7 +286,7 @@ again:
     }
     if (tvp) {
 #ifndef AFS_DARWIN80_ENV /* KPI callers don't need a usecount reference */
-	osi_vnhold(tvp, 0);
+	osi_Assert(osi_vnhold(tvp) == 0);
 	AFS_GUNLOCK();
 	vn_lock(AFSTOV(tvp), LK_EXCLUSIVE | LK_RETRY, p);
 	AFS_GLOCK();
