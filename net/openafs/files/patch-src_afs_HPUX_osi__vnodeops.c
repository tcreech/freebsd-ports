--- src/afs/HPUX/osi_vnodeops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/HPUX/osi_vnodeops.c
@@ -882,60 +882,6 @@ afspgin_io(vfspage_t * vm_info, struct vnode *devvp, p
     space_t space = VM_SPACE(vm_info);
     int num_io = VM_GET_NUM_IO(vm_info);
 
-#ifdef notdef			/* Not used in AFS */
-    /*
-     * With VM_READ_AHEAD_ALLOWED() macro, check if read-ahead should
-     * be used in this case.
-     *
-     * Unlike UFS, NFS does not start the faulting page I/O
-     * asynchronously. Why?  Asynchronous requests are handled by the
-     * biod's.  It doesn't make sense to queue up the faulting request
-     * behind other asynchrnous requests.  This is not true for UFS
-     * where the asynchrnous request is immediately handled.
-     */
-
-    if ((VM_READ_AHEAD_ALLOWED(vm_info)) && (nfs_read_ahead_on)
-	&& (NFS_DO_READ_AHEAD) && (should_do_read_ahead(prp, vaddr))) {
-
-	pgcnt_t max_rhead_io;
-	caddr_t rhead_vaddr;
-	pgcnt_t total_rheads_allowed;
-
-	/*
-	 * Determine the maximum amount of read-ahead I/O.
-	 */
-	total_rheads_allowed = maxpagein - count;
-
-	/*
-	 * If the count is less than a block, raise it to one.
-	 */
-	if (total_rheads_allowed < bpages)
-	    total_rheads_allowed = bpages;
-
-	max_rhead_io = total_rheads_allowed;
-	rhead_vaddr = VM_MAPPED_ADDR(vm_info) + (count * NBPG);
-	error =
-	    nfs_read_ahead(vm_info->vp, prp, wrt, space, rhead_vaddr,
-			   &max_rhead_io);
-
-	/*
-	 * Set the next fault location.  If read_ahead launches any
-	 * I/O it will adjust it accordingly.
-	 */
-	vm_info->prp->p_nextfault = vm_info->startindex + count;
-
-	/*
-	 * Now perform the faulting I/O synchronously.
-	 */
-	vm_unlock(vm_info);
-
-	error =
-	    syncpageio((swblk_t) VM_GET_IO_STARTBLK(vm_info, 0),
-		       VM_MAPPED_SPACE(vm_info), VM_MAPPED_ADDR(vm_info),
-		       (int)ptob(count), B_READ, devvp,
-		       B_vfs_pagein | B_pagebf, VM_REGION(vm_info));
-    } else
-#endif
     {
 	virt_addr = VM_MAPPED_ADDR(vm_info);
 	vm_unlock(vm_info);
@@ -1199,23 +1145,7 @@ afs_pagein(vp, prp, wrt, space, vaddr, ret_startindex)
     if (retval == VM_PAGE_PRESENT)
 	return (count);
 
-#if 0
     /*
-     * The definition of krusage_cntr_t is in h/kmetric.h, which
-     * is not shipped.  Since it's just statistics, we punt and do
-     * not update it.  If it's a problem we'll need to get HP to export
-     * an interface that we can use to increment the counter.
-     */
-
-    /* It's a real fault, not a reclaim */
-    {
-	krusage_cntr_t *temp;
-	temp = kt_cntrp(u.u_kthreadp);
-	temp->krc_majflt++;
-    }
-#endif
-
-    /*
      * Tell VM where the I/O intends to start.  This may be different
      * from the faulting point.
      */
@@ -1895,32 +1825,6 @@ afs_vm_checkpage(vp, args, pgindx, cur_data)
 	VM_SETFS_FLAGS(args, PAGEOUT_TRUNCATE);
 	return (1);
     }
-#ifdef notdef
-    if ((args->vm_flags & PAGEOUT_VHAND)
-	&& (!(args->vm_flags & PAGEOUT_RESERVED))
-	&& (!(VM_IS_ZOMBIE(args)))) {
-	VASSERT(args->run == 0);
-	if (vm_reserve_malloc_memory(NFS_PAGEOUT_MEM)) {
-	    /*
-	     * Got enough memory to pageout.  Mark the fact that we did
-	     * a sysprocmemreserve(), so that we can sysprocmemunreserve() it
-	     * later (in remote_pageout()).
-	     */
-	    args->vm_flags |= PAGEOUT_RESERVED;
-	} else {
-	    /*
-	     * We do not have enough memory to do this pageout.  By
-	     * definition, we do not yet have a run, so we just unlock
-	     * this page and tell foreach_valid() to continue scanning.
-	     * If we come across another dirty page, we will try to
-	     * reserve memory again.  That is okay, in fact some memory
-	     * may have freed up (as earlier pageouts complete under
-	     * interrupt).
-	     */
-	    return 1;
-	}
-    }
-#endif
     return (0);
 }
 
@@ -2042,12 +1946,6 @@ afs_ioctl(vp, com, data, flag, cred)
 
     /* The call must be a VICEIOCTL call */
     if (((com >> 8) & 0xff) == 'V') {
-#ifdef notdef
-	/* AFS_COPYIN returns error 14. Copy data in instead */
-	AFS_COPYIN(data, (caddr_t) & afsioctl, sizeof(afsioctl), error);
-	if (error)
-	    return (error);
-#endif
 	ai = (struct afs_ioctl *)data;
 	afsioctl.in = ai->in;
 	afsioctl.out = ai->out;
@@ -2512,10 +2410,6 @@ afsHashGarbageCollect()
 	}
 	hashUnlock(hashTable[index].lock);
     }
-#if 0
-    if (!foundFlag)
-	osi_Panic("afs: SEMA HashTable full\n");
-#endif
 }
 
 #endif /* AFS_SV_SEMA_HASH */
