--- src/config/stds.h.orig	2021-01-14 21:08:41 UTC
+++ src/config/stds.h
@@ -128,30 +128,6 @@ typedef afs_uint32 afs_uintmax_t;
  * some assistence in this matter.  The hyper type is supposed to be compatible
  * with the afsHyper type: the same macros will work on both. */
 
-#if 0
-
-typedef unsigned long afs_hyper_t;
-
-#define	hcmp(a,b)	((a) < (b) ? -1 : ((a) > (b) ? 1 : 0))
-#define	hsame(a,b)	((a) == (b))
-#define	hiszero(a)	((a) == 0)
-#define	hfitsin32(a)	((a) & 0xffffffff00000000) == 0)
-#define	hset(a,b)	((a) = (b))
-#define	hzero(a)	((a) = 0)
-#define	hones(a)	((a) = ~((unsigned long)0))
-#define	hget32(i,a)	((i) = (unsigned int)(a))
-#define	hget64(hi,lo,a)	((lo) = ((unsigned int)(a)), (hi) = ((a) & (0xffffffff00000000)))
-#define	hset32(a,i)	((a) = ((unsigned int)(i)))
-#define	hset64(a,hi,lo)	((a) = (((hi) << 32) | (lo)))
-#define hgetlo(a)	((a) & 0xffffffff)
-#define hgethi(a)	(((unsigned int)(a)) >> 32)
-#define	hadd(a,b)	((a) += (b))
-/* XXX */
-#define	hadd32(a,b)	((a) += (b))
-#define hshlft(a,n)     ((a)<<(n))
-
-#else /* 0*/
-
 typedef struct afs_hyper_t {	/* unsigned 64 bit integers */
     unsigned int high;
     unsigned int low;
@@ -199,7 +175,6 @@ typedef struct afs_hyper_t {	/* unsigned 64 bit intege
      (a).low += (int)(i))
 
 #define hadd(a,b) (hadd32(a,(b).low), (a).high += (b).high)
-#endif /* 0 */
 
 #if !defined(KERNEL) || defined(UKERNEL)
 #ifndef AFS_NT40_ENV
