--- src/afs/LINUX/osi_machdep.h.orig	2021-07-29 10:24:31 UTC
+++ src/afs/LINUX/osi_machdep.h
@@ -124,7 +124,6 @@ static inline time_t osi_Time(void) {
 #undef gop_lookupname_user
 #define gop_lookupname_user osi_lookupname
 
-#define osi_vnhold(V, N) do { VN_HOLD(AFSTOV(V)); } while (0)
 #define VN_HOLD(V) osi_Assert(igrab((V)) == (V))
 #define VN_RELE(V) iput((V))
 
