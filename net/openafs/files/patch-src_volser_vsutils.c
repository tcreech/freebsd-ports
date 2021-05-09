--- src/volser/vsutils.c.orig	2021-01-14 21:08:41 UTC
+++ src/volser/vsutils.c
@@ -136,6 +136,7 @@ VLDB_GetEntryByID(afs_uint32 volid, afs_int32 voltype,
 	    ovlentry_to_nvlentry(&oentry, entryp);
 	return code;
     }
+    memset(entryp, 0, sizeof(*entryp));	    /* ensure padding is cleared */
     code = ubik_VL_GetEntryByIDN(cstruct, 0, volid, voltype, entryp);
     if (newvlserver == vltype_unknown) {
 	if (code == RXGEN_OPCODE) {
@@ -161,6 +162,7 @@ VLDB_GetEntryByName(char *namep, struct nvldbentry *en
 	    ovlentry_to_nvlentry(&oentry, entryp);
 	return code;
     }
+    memset(entryp, 0, sizeof(*entryp));	    /* ensure padding is cleared */
     code = ubik_VL_GetEntryByNameN(cstruct, 0, namep, entryp);
     if (newvlserver == vltype_unknown) {
 	if (code == RXGEN_OPCODE) {
