--- src/lwp/process.default.s.orig	2021-01-14 21:08:41 UTC
+++ src/lwp/process.default.s
@@ -1042,22 +1042,11 @@ returnto:
 
 #ifdef __alpha
 /* Code for DEC Alpha architecture */
-#ifdef	AFS_OSF_ENV
+#if defined(AFS_XBSD_ENV)
 #include <machine/asm.h>
-#include <machine/regdef.h>
-#define	fs0	$f2
-#define	fs1	$f3
-#define	fs2	$f4
-#define	fs3	$f5
-#define	fs4	$f6
-#define	fs5	$f7
-#define	fs6	$f8
-#define	fs7	$f9
-#elif defined(AFS_XBSD_ENV)
-#include <machine/asm.h>
-#else	/* !OSF && !XBSD */
+#else	/* !XBSD */
 #include <mach/alpha/asm.h>
-#endif	/* OSF */
+#endif	/* !XBSD */
 
 #define FRAMESIZE ((8*8)+8+(7*8))
 #define floats 0
@@ -1065,15 +1054,8 @@ returnto:
 #define returnaddr (FRAMESIZE-8)
 #define topstack 0
 
-#ifdef AFS_OSF_ENV
-IMPORT(PRE_Block,4)
-#endif
 .align	4
-#ifdef	AFS_OSF_ENV
-NESTED(savecontext,FRAMESIZE,ra)
-#else	/* OSF */
 NESTED(savecontext,3,FRAMESIZE,ra,0x0400f700,0x000003fc)
-#endif	/* OSF */
 	ldgp	gp,0(pv)
 	lda	t0, 1(zero)
 	stl	t0, PRE_Block
@@ -1110,11 +1092,7 @@ samestack:
 	jsr	ra,(pv),0		/* off we go */
 	END(savecontext)
 
-#ifdef	AFS_OSF_ENV
-LEAF(returnto)
-#else	
 LEAF(returnto,1)
-#endif	
 	ldgp	gp,0(pv)
 
 	.prologue	1
