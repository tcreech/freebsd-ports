--- src/venus/fstrace.c.orig	2021-01-14 21:08:41 UTC
+++ src/venus/fstrace.c
@@ -24,7 +24,7 @@
 #include <afs/vice.h>
 #include <afs/sys_prototypes.h>
 
-#if defined(AFS_OSF_ENV) || defined(AFS_SGI61_ENV) || (defined(AFS_AIX51_ENV) && defined(AFS_64BIT_KERNEL))
+#if defined(AFS_SGI61_ENV) || (defined(AFS_AIX51_ENV) && defined(AFS_64BIT_KERNEL))
 /* For SGI 6.2, this is changed to 1 if it's a 32 bit kernel. */
 int afs_icl_sizeofLong = 2;
 #else
@@ -296,10 +296,7 @@ DisplayRecord(FILE *outFilep, afs_int32 *alp, afs_int3
 		printfParms[pfpix] <<= 32;
 		printfParms[pfpix] |= alp[pix + 1];
 	    }
-#elif defined(AFS_OSF_ENV)
-	    printfParms[pfpix] = alp[pix + 1];
-	    printfParms[pfpix] |= (alp[pix] <<= 32);
-#else /* !AFS_OSF_ENV && !AFS_SGI61_ENV */
+#else /* !AFS_SGI61_ENV */
 	    printfParms[pfpix] = alp[pix];
 #endif
 	    pfpix++;
@@ -510,9 +507,6 @@ DisplayRecord(FILE *outFilep, afs_int32 *alp, afs_int3
 
 
 #include <locale.h>
-#ifdef	AFS_OSF_ENV
-#include <limits.h>
-#endif
 #include <nl_types.h>
 
 #define FACILITY_CODE_MASK          0xF0000000
@@ -759,10 +753,6 @@ icl_DumpKernel(FILE *outFilep, char *setname)
 	    /* print the record */
 	    DisplayRecord(outFilep, &bufferp[ix], rlength);
 	    ix += rlength;
-#ifdef notdef
-	    /* obsolete: read entire buffer first */
-	    i += rlength;	/* update cookie value, too */
-#endif
 	}			/* for loop displaying buffer */
     }				/* for loop over all logs */
 
