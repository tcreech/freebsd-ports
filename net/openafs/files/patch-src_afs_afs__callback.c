--- src/afs/afs_callback.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/afs_callback.c
@@ -447,9 +447,6 @@ loop1:
 			    afs_osi_Sleep(&tvc->f.states);
 			    goto loop1;
 			}
-#if     defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV)  || defined(AFS_HPUX_ENV) || defined(AFS_LINUX20_ENV)
-			AFS_FAST_HOLD(tvc);
-#else
 #ifdef AFS_DARWIN80_ENV
 			if (tvc->f.states & CDeadVnode) {
 			    ReleaseReadLock(&afs_xvcache);
@@ -466,9 +463,10 @@ loop1:
 			    continue;
 			}
 #else
-			AFS_FAST_HOLD(tvc);
+			if (osi_vnhold(tvc) != 0) {
+			    continue;
+			}
 #endif
-#endif
 			ReleaseReadLock(&afs_xvcache);
 			afs_StaleVCacheFlags(tvc, 0, CUnique | CBulkFetching);
 			afs_allCBs++;
@@ -530,9 +528,6 @@ loop2:
 			afs_osi_Sleep(&tvc->f.states);
 			goto loop2;
 		    }
-#if     defined(AFS_SGI_ENV) || defined(AFS_SUN5_ENV)  || defined(AFS_HPUX_ENV) || defined(AFS_LINUX20_ENV)
-		    AFS_FAST_HOLD(tvc);
-#else
 #ifdef AFS_DARWIN80_ENV
 		    if (tvc->f.states & CDeadVnode) {
 			ReleaseReadLock(&afs_xvcache);
@@ -549,9 +544,10 @@ loop2:
 			continue;
 		    }
 #else
-		    AFS_FAST_HOLD(tvc);
+		    if (osi_vnhold(tvc) != 0) {
+			continue;
+		    }
 #endif
-#endif
 		    ReleaseReadLock(&afs_xvcache);
 		    afs_StaleVCacheFlags(tvc, 0, CUnique | CBulkFetching);
 		    afs_Trace3(afs_iclSetp, CM_TRACE_CALLBACK,
@@ -984,7 +980,9 @@ afs_RXCallBackServer(void)
     /*
      * Donate this process to Rx.
      */
+    AFS_GUNLOCK();
     rx_ServerProc(NULL);
+    AFS_GLOCK();
     return (0);
 
 }				/*afs_RXCallBackServer */
@@ -1639,135 +1637,12 @@ SRXAFSCB_TellMeAboutYourself(struct rx_call *a_call,
 }
 
 
-#if 0 && defined(AFS_LINUX24_ENV)
-extern struct vcache *afs_globalVp;
-
-int recurse_dcache_parent(parent, a_index, addr, inode, flags, time, fileName)
-     struct dentry * parent;
-    afs_int32 a_index;
-    afs_int32 *addr;
-    afs_int32 *inode;
-    afs_int32 *flags;
-    afs_int32 *time;
-    char ** fileName;
-{
-	struct dentry *this_parent = parent;
-	struct list_head *next;
-	int found = 0;
-	struct dentry *dentry;
-
-repeat:
-	next = this_parent->d_subdirs.next;
-resume:
-	while (next != &this_parent->d_subdirs) {
-		struct list_head *tmp = next;
-		dentry = list_entry(tmp, struct dentry, d_child);
-		if (a_index == 0)
-		  goto searchdone3;
-		a_index--;
-		next = tmp->next;
-		/*
-		 * Descend a level if the d_subdirs list is non-empty.
-		 */
-		if (!list_empty(&dentry->d_subdirs)) {
-			this_parent = dentry;
-			goto repeat;
-		}
-	}
-	/*
-	 * All done at this level ... ascend and resume the search.
-	 */
-	if (this_parent != parent) {
-		next = this_parent->d_child.next;
-		this_parent = this_parent->d_parent;
-		goto resume;
-	}
-	goto ret;
-
- searchdone3:
-    if (d_unhashed(dentry))
-      *flags = 1;
-    else
-      *flags = 0;
-
-    *fileName = afs_strdup(dentry->d_name.name?dentry->d_name.name:"");
-    *inode = ITOAFS(dentry->d_inode);
-    *addr = atomic_read(&(dentry)->d_count);
-    *time = dentry->d_time;
-
-    return 0;
- ret:
-    return 1;
-}
-#endif
-
 int
 SRXAFSCB_GetDE(struct rx_call *a_call, afs_int32 a_index, afs_int32 *addr,
 	       afs_int32 *inode, afs_int32 *flags, afs_int32 *time,
 	       char ** fileName)
 { /*SRXAFSCB_GetDE*/
     int code = 0;				/*Return code*/
-#if 0 && defined(AFS_LINUX24_ENV)
-    int i;			/*Loop variable*/
-    struct vcache *tvc = afs_globalVp;
-    struct dentry *dentry;
-    struct list_head *cur, *head = &(AFSTOI(tvc))->i_dentry;
-
-#ifdef RX_ENABLE_LOCKS
-    AFS_GLOCK();
-#endif /* RX_ENABLE_LOCKS */
-
-#if defined(AFS_LINUX24_ENV)
-    spin_lock(&dcache_lock);
-#endif
-
-    cur = head;
-    while ((cur = cur->next) != head) {
-      dentry = list_entry(cur, struct dentry, d_alias);
-
-      dget_locked(dentry);
-
-#if defined(AFS_LINUX24_ENV)
-      spin_unlock(&dcache_lock);
-#endif
-      if (a_index == 0)
-	goto searchdone2;
-      a_index--;
-
-      if (recurse_dcache_parent(dentry, a_index, addr, inode, flags, time, fileName) == 0) {
-	dput(dentry);
-	code = 0;
-	goto fcnDone;
-      }
-      dput(dentry);
-    }
- searchdone2:
-    if (cur == head) {
-	/*Past EOF*/
-	code = 1;
-	*fileName = afs_strdup("");
-	goto fcnDone;
-    }
-
-    if (d_unhashed(dentry))
-      *flags = 1;
-    else
-      *flags = 0;
-
-    *fileName = afs_strdup(dentry->d_name.name?dentry->d_name.name:"");
-    *inode = ITOAFS(dentry->d_inode);
-    *addr = atomic_read(&(dentry)->d_count);
-    *time = dentry->d_time;
-
-    dput(dentry);
-    code = 0;
-
-fcnDone:
-
-#ifdef RX_ENABLE_LOCKS
-    AFS_GUNLOCK();
-#endif /* RX_ENABLE_LOCKS */
-#endif
     return(code);
 
 } /*SRXAFSCB_GetDE*/
