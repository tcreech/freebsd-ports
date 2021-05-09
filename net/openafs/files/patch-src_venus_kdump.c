--- src/venus/kdump.c.orig	2021-01-14 21:08:41 UTC
+++ src/venus/kdump.c
@@ -116,45 +116,6 @@ struct ncp_sb_info {
 
 #include <afs/stds.h>
 
-#if defined(AFS_OSF_ENV)
-#define	KERNEL
-#define	UNIX_LOCKS
-#define	_KERNEL	1
-#ifdef	_KERN_LOCK_H_
-#include FFFFF
-#endif
-#include <kern/lock.h>
-#include <sys/vnode.h>
-#include <arch/alpha/pmap.h>
-
-/*
- * beginning with DUX 4.0A, the system header files define the macros
- *
- * KSEG_TO_PHYS()
- * IS_KSEG_VA()
- * IS_SEG1_VA()
- *
- * to be calls to the kernel functions
- *
- * kseg_to_phys()
- * is_kseg_va()
- * is_seg1_va()
- *
- * when _KERNEL is defined, and expressions otherwise.  Since need
- * to define _KERNEL, we redefine these kernel functions as macros
- * for the expressions that we would have gotten if _KERNEL had not
- * been defined.  Yes, this duplicates code from the header files, but
- * there's no simple way around it.
- */
-
-#define kseg_to_phys(addr) ((vm_offset_t)(addr) - UNITY_BASE)
-#define is_kseg_va(x)   (((unsigned long)(x) & SEG1_BASE) == UNITY_BASE)
-#define is_seg1_va(x)   (((unsigned long)(x) & SEG1_BASE) == SEG1_BASE)
-
-#undef	KERNEL
-#undef  _KERNEL
-#endif
-
 #ifdef	AFS_SUN5_ENV /*XXXXX*/
 #include <sys/t_lock.h>
 struct vnode foo;
@@ -203,7 +164,7 @@ struct vnode foo;
 # include <netinet/in.h>	/* struct in_addr */
 #endif
 
-#if defined(AFS_SGI_ENV) || defined(AFS_OSF_ENV)
+#if defined(AFS_SGI_ENV)
 #ifdef       AFS_SGI_ENV
 #include <sys/vnode.h>
 #endif /* AFS_SGI_ENV */
@@ -258,12 +219,6 @@ struct vnode foo;
 struct timezone {
     int a, b;
 };
-#if 0				/*ndef AFS_ALPHA_LINUX20_ENV */
-typedef struct timeval {
-    int tv_sec;
-    int tv_usec;
-} timeval_t;			/* Needed here since KERNEL defined. */
-#endif /*AFS_ALPHA_LINUX20_ENV */
 #if defined(WORDS_BIGENDIAN)
 #define _LINUX_BYTEORDER_BIG_ENDIAN_H
 #else
@@ -355,15 +310,6 @@ typedef kmutex_t afs_kmutex_t;
 typedef kcondvar_t afs_kcondvar_t;
 #endif /* AFS_SUN5_ENV */
 
-#ifdef AFS_DUX40_ENV
-#define RX_ENABLE_LOCKS
-typedef struct {
-    unsigned long lock;
-    void *owner;
-} afs_kmutex_t;
-typedef int afs_kcondvar_t;
-#endif /* AFS_DUX40_ENV */
-
 #ifdef AFS_HPUX110_ENV
 #define RX_ENABLE_LOCKS
 typedef struct {
@@ -449,15 +395,6 @@ extern off64_t lseek64();
 
 #undef	KERNEL
 
-#if defined(AFS_OSF_ENV) && !defined(v_count)
-#define v_count         v_usecount
-#endif
-
-#ifdef	AFS_OSF_ENV
-#define	KERNELBASE	0x80000000
-#define	coreadj(x)	((int)x - KERNELBASE)
-#endif
-
 #if defined(AFS_SGI_ENV)
 #define UNIX "/unix"
 #else
@@ -1043,15 +980,6 @@ findsym(char *sname, off_t * offset)
 		    errno, strerror(errno));
 	    exit(1);
 	}
-#if	defined(AFS_OSF_ENV)
-	if (mem) {
-	    long X;
-
-	    X = coreadj(request[0].n_value);
-	    request[0].n_value = X;
-	}
-#endif /* defined(AFS_OSF_ENV) */
-
 	*offset = request[0].n_value;
 	if (!request[0].n_value) {
 	    printf("(WARNING) Couldn't find %s in %s. Proceeding..\n", sname,
@@ -2197,79 +2125,6 @@ readmem(kmem, buf, vad, len)
 }
 #endif
 
-#ifdef	AFS_OSF_ENV
-static
-read_addr(int fd, unsigned long addr, unsigned long *val)
-{
-    if (lseek(fd, addr, SEEK_SET) == -1)
-	return (0);
-    if (read(fd, val, sizeof(long)) != sizeof(long))
-	return (0);
-    return (1);
-}
-
-static pt_entry_t *ptes = NULL;
-static
-addr_to_offset(unsigned long addr, unsigned long *ret, int fd)
-{
-    off_t symoff;
-    pt_entry_t pte, *val;
-    char *str, *ptr;
-
-    if (IS_SEG1_VA(addr)) {
-	if (ptes == NULL) {
-	    int i, loc;
-	    unsigned long loc1, loc2[2];
-	    findsym("kernel_pmap", &symoff);
-	    loc1 = coreadj(symoff);
-	    /*printf("ptes=%lx -> %lx\n", symoff, loc1); */
-	    if (lseek(fd, loc1, L_SET /*0 */ ) != loc1) {
-		perror("lseek");
-		exit(1);
-	    }
-	    if ((i = read(fd, (char *)&loc1, sizeof(long))) != sizeof(long)) {
-		printf("Read of kerne_map failed\n");
-		return;		/*exit(1); */
-	    }
-	    loc = loc1;
-	    /*printf("loc1 %lx -> %lx\n",  loc1, loc); */
-	    if (lseek(fd, loc, L_SET /*0 */ ) != loc) {
-		perror("lseek");
-		exit(1);
-	    }
-	    if ((i =
-		 read(fd, (char *)loc2,
-		      2 * sizeof(long))) != 2 * sizeof(long)) {
-		printf("Read of kerne_map failed\n");
-		return;		/*exit(1); */
-	    }
-	    ptes = (pt_entry_t *) loc2[1];
-	    /*printf("ptes=%lx\n", ptes); */
-
-	}
-	if (!addr_to_offset
-	    ((unsigned long)(ptes + LEVEL1_PT_OFFSET(addr)),
-	     (unsigned long *)&val, fd))
-	    return (0);
-	if (!read_addr(fd, (unsigned long)val, (unsigned long *)&pte))
-	    return (0);
-	val = ((pt_entry_t *) PTETOPHYS(&pte)) + LEVEL2_PT_OFFSET(addr);
-	if (!read_addr(fd, (unsigned long)val, (unsigned long *)&pte))
-	    return (0);
-	val = ((pt_entry_t *) PTETOPHYS(&pte)) + LEVEL3_PT_OFFSET(addr);
-	if (!read_addr(fd, (unsigned long)val, (unsigned long *)&pte))
-	    return (0);
-	*ret = PTETOPHYS(&pte) + (addr & ((1 << PGSHIFT) - 1));
-	return (1);
-    } else if (IS_KSEG_VA(addr)) {
-	*ret = KSEG_TO_PHYS(addr);
-	return (1);
-    } else {
-	return (0);
-    }
-}
-#endif
-
 #ifndef AFS_KDUMP_LIB
 void
 kread(int kmem, off_t loc, void *buf, KDUMP_SIZE_T len)
@@ -2278,19 +2133,6 @@ kread(int kmem, off_t loc, void *buf, KDUMP_SIZE_T len
 
     memset(buf, 0, len);
 
-#ifdef	AFS_OSF_ENV
-    if (mem) {
-	unsigned long ret;
-	i = addr_to_offset(loc, &ret, kmem);
-	if (i == 1)
-	    loc = ret;
-	else {
-	    unsigned long loc1;
-	    loc1 = coreadj(loc);
-	    loc = loc1;
-	}
-    }
-#else
 #if	defined(sparc) && !defined(__linux__)
 #ifndef AFS_SUN5_ENV
     if (mem) {
@@ -2301,7 +2143,6 @@ kread(int kmem, off_t loc, void *buf, KDUMP_SIZE_T len
     }
 #endif
 #endif
-#endif
 #if	! defined(AFS_SUN5_ENV)
 #if defined(AFS_SGI61_ENV) && !defined(AFS_32BIT_KERNEL_ENV)
     if (lseek64(kmem, loc, L_SET /*0 */ ) != loc)
@@ -2997,17 +2838,6 @@ print_vlru(int kmem)
     printf("\n\nPrinting vcache VLRU info (oldest first)...\n\n");
     findsym("afs_cacheStats", &symoff);
     kread(kmem, symoff, (char *)&maxvcount, sizeof maxvcount);
-#ifdef	AFS_OSF_ENV
-    findsym("afs_maxvcount", &symoff);
-    kread(kmem, symoff, (char *)&maxvcount, sizeof maxvcount);
-    findsym("afs_vcount", &symoff);
-    kread(kmem, symoff, (char *)&vcount, sizeof vcount);
-    findsym("max_vnodes", &symoff);
-    kread(kmem, symoff, (char *)&nvnode, sizeof nvnode);
-    printf("max number of vcache entries = %d\n", maxvcount);
-    printf("number of vcaches in use = %d\n", vcount);
-    printf("total number of system vnode entries = %d\n", nvnode);
-#endif
     findsym("VLRU", &symoff);
     kread(kmem, symoff, (char *)&VLRU, sizeof VLRU);
     vlru_addr = (u_long) symoff;
@@ -3234,11 +3064,9 @@ print_global_afs_cache(int kmem)
     findsym("afs_mariner", &symoff);
     kread(kmem, symoff, (char *)&count, sizeof count);
     printf("\tafs_mariner = 0x%x\n", count);
-#ifndef	AFS_OSF_ENV
     findsym("freeVCList", &symoff);
     kread(kmem, symoff, (char *)&count, sizeof count);
     printf("\tafs_freeVCList = 0x%x XXX\n", count);
-#endif
     findsym("afs_freeDCList", &symoff);
     kread(kmem, symoff, (char *)&count, sizeof count);
     printf("\tfreeDCList = 0x%x\n", count);
@@ -4463,163 +4291,4 @@ print_cmstats(struct afs_CMStats *cmp)
 #endif
 }
 
-#endif
-#if 0
-#define OffsetOf(s,mem)	((long)(&(((s *)0)->mem)))
-#define SizeOf(s,mem)	((long)sizeof(((s *)0)->mem))
-#define values(s,mem)	OffsetOf(s,mem), SizeOf(s,mem)
-
-print_struct_vcache_offsets()
-{
-    printf("struct vcache.v              offset = %ld, size = %ld\n",
-	   values(struct vcache, v));
-    printf("struct vcache.vlruq          offset = %ld, size = %ld\n",
-	   values(struct vcache, vlruq));
-    printf("struct vcache.nextfree       offset = %ld, size = %ld\n",
-	   values(struct vcache, nextfree));
-    printf("struct vcache.hnext          offset = %ld, size = %ld\n",
-	   values(struct vcache, hnext));
-    printf("struct vcache.fid            offset = %ld, size = %ld\n",
-	   values(struct vcache, fid));
-    printf("struct vcache.m              offset = %ld, size = %ld\n",
-	   values(struct vcache, m));
-    printf("struct vcache.lock           offset = %ld, size = %ld\n",
-	   values(struct vcache, lock));
-    printf("struct vcache.parentVnode    offset = %ld, size = %ld\n",
-	   values(struct vcache, parentVnode));
-    printf("struct vcache.parentUnique   offset = %ld, size = %ld\n",
-	   values(struct vcache, parentUnique));
-    printf("struct vcache.mvid           offset = %ld, size = %ld\n",
-	   values(struct vcache, mvid));
-    printf("struct vcache.linkData       offset = %ld, size = %ld\n",
-	   values(struct vcache, linkData));
-    printf("struct vcache.flushDV        offset = %ld, size = %ld\n",
-	   values(struct vcache, flushDV));
-    printf("struct vcache.mapDV          offset = %ld, size = %ld\n",
-	   values(struct vcache, mapDV));
-    printf("struct vcache.truncPos       offset = %ld, size = %ld\n",
-	   values(struct vcache, truncPos));
-    printf("struct vcache.callback       offset = %ld, size = %ld\n",
-	   values(struct vcache, callback));
-    printf("struct vcache.cbExpires      offset = %ld, size = %ld\n",
-	   values(struct vcache, cbExpires));
-    printf("struct vcache.callsort       offset = %ld, size = %ld\n",
-	   values(struct vcache, callsort));
-    printf("struct vcache.Access         offset = %ld, size = %ld\n",
-	   values(struct vcache, Access));
-    printf("struct vcache.anyAccess      offset = %ld, size = %ld\n",
-	   values(struct vcache, anyAccess));
-    printf("struct vcache.last_looker    offset = %ld, size = %ld\n",
-	   values(struct vcache, last_looker));
-    printf("struct vcache.activeV        offset = %ld, size = %ld\n",
-	   values(struct vcache, activeV));
-    printf("struct vcache.slocks         offset = %ld, size = %ld\n",
-	   values(struct vcache, slocks));
-    printf("struct vcache.opens          offset = %ld, size = %ld\n",
-	   values(struct vcache, opens));
-    printf("struct vcache.execsOrWriters offset = %ld, size = %ld\n",
-	   values(struct vcache, execsOrWriters));
-    printf("struct vcache.flockCount     offset = %ld, size = %ld\n",
-	   values(struct vcache, flockCount));
-    printf("struct vcache.mvstat         offset = %ld, size = %ld\n",
-	   values(struct vcache, mvstat));
-    printf("struct vcache.states         offset = %ld, size = %ld\n",
-	   values(struct vcache, states));
-    printf("struct vcache.quick          offset = %ld, size = %ld\n",
-	   values(struct vcache, quick));
-    printf("struct vcache.symhintstamp   offset = %ld, size = %ld\n",
-	   values(struct vcache, symhintstamp));
-    printf("struct vcache.h1             offset = %ld, size = %ld\n",
-	   values(struct vcache, h1));
-    printf("struct vcache.lastr          offset = %ld, size = %ld\n",
-	   values(struct vcache, lastr));
-    printf("struct vcache.vc_rwlockid    offset = %ld, size = %ld\n",
-	   values(struct vcache, vc_rwlockid));
-    printf("struct vcache.vc_locktrips   offset = %ld, size = %ld\n",
-	   values(struct vcache, vc_locktrips));
-    printf("struct vcache.vc_rwlock      offset = %ld, size = %ld\n",
-	   values(struct vcache, vc_rwlock));
-    printf("struct vcache.mapcnt         offset = %ld, size = %ld\n",
-	   values(struct vcache, mapcnt));
-    printf("struct vcache.cred           offset = %ld, size = %ld\n",
-	   values(struct vcache, cred));
-    printf("struct vcache.vc_bhv_desc    offset = %ld, size = %ld\n",
-	   values(struct vcache, vc_bhv_desc));
-    printf("struct vcache.vc_error       offset = %ld, size = %ld\n",
-	   values(struct vcache, vc_error));
-    printf("struct vcache.xlatordv       offset = %ld, size = %ld\n",
-	   values(struct vcache, xlatordv));
-    printf("struct vcache.uncred         offset = %ld, size = %ld\n",
-	   values(struct vcache, uncred));
-    printf("struct vcache.asynchrony     offset = %ld, size = %ld\n",
-	   values(struct vcache, asynchrony));
-}
-
-print_struct_vnode_offsets()
-{
-    printf("struct vnode.v_list           offset = %ld, size = %ld\n",
-	   values(struct vnode, v_list));
-    printf("struct vnode.v_flag           offset = %ld, size = %ld\n",
-	   values(struct vnode, v_flag));
-    printf("struct vnode.v_count          offset = %ld, size = %ld\n",
-	   values(struct vnode, v_count));
-    printf("struct vnode.v_listid         offset = %ld, size = %ld\n",
-	   values(struct vnode, v_listid));
-    printf("struct vnode.v_intpcount      offset = %ld, size = %ld\n",
-	   values(struct vnode, v_intpcount));
-    printf("struct vnode.v_type           offset = %ld, size = %ld\n",
-	   values(struct vnode, v_type));
-    printf("struct vnode.v_rdev           offset = %ld, size = %ld\n",
-	   values(struct vnode, v_rdev));
-    printf("struct vnode.v_vfsmountedhere offset = %ld, size = %ld\n",
-	   values(struct vnode, v_vfsmountedhere));
-    printf("struct vnode.v_vfsp           offset = %ld, size = %ld\n",
-	   values(struct vnode, v_vfsp));
-    printf("struct vnode.v_stream         offset = %ld, size = %ld\n",
-	   values(struct vnode, v_stream));
-    printf("struct vnode.v_filocks        offset = %ld, size = %ld\n",
-	   values(struct vnode, v_filocks));
-    printf("struct vnode.v_filocksem      offset = %ld, size = %ld\n",
-	   values(struct vnode, v_filocksem));
-    printf("struct vnode.v_number         offset = %ld, size = %ld\n",
-	   values(struct vnode, v_number));
-    printf("struct vnode.v_bh             offset = %ld, size = %ld\n",
-	   values(struct vnode, v_bh));
-    printf("struct vnode.v_namecap        offset = %ld, size = %ld\n",
-	   values(struct vnode, v_namecap));
-    printf("struct vnode.v_hashp          offset = %ld, size = %ld\n",
-	   values(struct vnode, v_hashp));
-    printf("struct vnode.v_hashn          offset = %ld, size = %ld\n",
-	   values(struct vnode, v_hashn));
-    printf("struct vnode.v_mreg           offset = %ld, size = %ld\n",
-	   values(struct vnode, v_mreg));
-    printf("struct vnode.v_mregb          offset = %ld, size = %ld\n",
-	   values(struct vnode, v_mregb));
-    printf("struct vnode.v_pgcnt          offset = %ld, size = %ld\n",
-	   values(struct vnode, v_pgcnt));
-    printf("struct vnode.v_dpages         offset = %ld, size = %ld\n",
-	   values(struct vnode, v_dpages));
-    printf("struct vnode.v_dpages_gen     offset = %ld, size = %ld\n",
-	   values(struct vnode, v_dpages_gen));
-    printf("struct vnode.v_dbuf           offset = %ld, size = %ld\n",
-	   values(struct vnode, v_dbuf));
-    printf("struct vnode.v_buf            offset = %ld, size = %ld\n",
-	   values(struct vnode, v_buf));
-    printf("struct vnode.v_bufgen         offset = %ld, size = %ld\n",
-	   values(struct vnode, v_bufgen));
-    printf("struct vnode.v_traceix        offset = %ld, size = %ld\n",
-	   values(struct vnode, v_traceix));
-    printf("struct vnode.v_buf_lock       offset = %ld, size = %ld\n",
-	   values(struct vnode, v_buf_lock));
-    printf("struct vnode.v_pc             offset = %ld, size = %ld\n",
-	   values(struct vnode, v_pc));
-#ifdef VNODE_TRACING
-    printf("struct vnode.v_trace          offset = %ld, size = %ld\n",
-	   values(struct vnode, v_trace));
-#endif
-#ifdef CKPT
-    printf("struct vnode.v_ckpt           offset = %ld, size = %ld\n",
-	   values(struct vnode, v_ckpt));
-#endif
-}
 #endif
