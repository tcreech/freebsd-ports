--- src/afs/FBSD/osi_groups.c.orig	2021-12-09 17:07:41 UTC
+++ src/afs/FBSD/osi_groups.c
@@ -68,32 +68,89 @@ Afs_xsetgroups(struct thread *td, struct setgroups_arg
     return code;
 }
 
+/**
+ * Take a PAG, and put it into the given array of gids. This is very similar to
+ * the one-group Solaris version.
+ *
+ * @param[in] pagvalue     The numeric id for the PAG to assign (must not be -1)
+ * @param[in] gidset       An array of gids
+ * @param[inout] a_ngroups How many entries in 'gidset' have valid gids
+ * @param[in] gidset_sz    The number of bytes allocated for 'gidset'
+ *
+ * @return error code
+ */
+static int
+afs_fbsd_pag_to_gidset(afs_uint32 pagvalue, gid_t *gidset, int *a_ngroups,
+              size_t gidset_sz)
+{
+    int i;
+    gid_t *gidslot = NULL;
+    int ngroups = *a_ngroups;
 
+    osi_Assert(pagvalue != -1);
+
+    /* See if we already have a PAG gid */
+    for (i = 0; i < ngroups; i++) {
+        if (((gidset[i] >> 24) & 0xff) == 'A') {
+            gidslot = &gidset[i];
+            break;
+        }
+    }
+
+    if (gidslot == NULL) {
+        /* If we don't already have a PAG, grow the groups list by one, and put
+         * our PAG in the new empty slot. */
+        if ((sizeof(gidset[0])) * (ngroups + 1) > gidset_sz) {
+            return E2BIG;
+        }
+        ngroups += 1;
+        gidslot = &gidset[ngroups-1];
+    }
+
+    /*
+     * As with newer Solaris releases, we cannot control the order of
+     * the supplemental groups list of a process, so we can't store PAG gids as
+     * the first two gids. To make finding a PAG gid easier to find, just use a
+     * single gid to represent a PAG, and just store the PAG id itself in
+     * there, like is currently done on Linux. Note that our PAG ids all start
+     * with the byte 0x41 ('A'), so we should not collide with anything. GIDs
+     * with the highest bit set are special (used for Windows SID mapping), but
+     * anything under that range should be fine.
+     * TODO: Is this sufficient to avoid a collision on FBSD too?
+     */
+    *gidslot = pagvalue;
+
+    *a_ngroups = ngroups;
+
+    return 0;
+}
+
 int
 setpag(struct thread *td, struct ucred **cred, afs_uint32 pagvalue,
        afs_uint32 * newpag, int change_parent)
 {
     gid_t *gidset;
+    struct proc *p;
     int gidset_len = ngroups_max + 1;
     int ngroups, code;
     int j;
 
     AFS_STATCNT(setpag);
+    p = td->td_proc;
     gidset = osi_Alloc(gidset_len * sizeof(gid_t));
+    PROC_LOCK(p);
     ngroups = afs_getgroups(*cred, gidset_len, gidset);
-    if (afs_get_pag_from_groups(gidset[1], gidset[2]) == NOPAG) {
-	/* We will have to shift grouplist to make room for pag */
-	if (ngroups + 2 > gidset_len) {
-	    return (E2BIG);
-	}
-	for (j = ngroups - 1; j >= 1; j--) {
-	    gidset[j + 2] = gidset[j];
-	}
-	ngroups += 2;
-    }
+
     *newpag = (pagvalue == -1 ? genpag() : pagvalue);
-    afs_get_groups_from_pag(*newpag, &gidset[1], &gidset[2]);
+
+    code = afs_fbsd_pag_to_gidset(*newpag, gidset, &ngroups, (gidset_len * sizeof(gid_t)));
+    if (code != 0)
+        goto done;
+
     code = afs_setgroups(td, cred, ngroups, gidset, change_parent);
+
+done:
+    PROC_UNLOCK(p);
     osi_Free(gidset, gidset_len * sizeof(gid_t));
     return code;
 }
@@ -118,5 +175,49 @@ static int
 afs_setgroups(struct thread *td, struct ucred **cred, int ngroups,
 	      gid_t * gidset, int change_parent)
 {
+    struct proc *p;
+    struct ucred *newcr;
+
+    AFS_STATCNT(afs_setgroups);
+
+    if (ngroups > ngroups_max) {
+        return EINVAL;
+    }
+
+
+    /* If we'd like to diverge from the parent (or other entities using the
+     * same cred) we must create and assign a new one. */
+    if (!change_parent) {
+        p = td->td_proc;
+	newcr = crget();
+	crcopy(newcr, *cred);
+	*cred = newcr;
+    }
+
+    crsetgroups(*cred, ngroups, gidset);
+
+    if (!change_parent) {
+	proc_set_cred(p, newcr);
+	crhold(newcr);
+    }
+
     return (0);
+}
+
+afs_int32
+osi_get_group_pag(struct ucred *cred) {
+    gid_t *gidset;
+    int ngroups;
+    int i;
+
+    gidset = cred->cr_groups;
+    ngroups = cred->cr_ngroups;
+
+    for (i = 0; i < ngroups; i++) {
+        if (((gidset[i] >> 24) & 0xff) == 'A') {
+	    return gidset[i];
+        }
+    }
+
+    return NOPAG;
 }
