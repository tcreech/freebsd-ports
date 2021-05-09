--- src/uss/uss_vol.c.orig	2021-01-14 21:08:41 UTC
+++ src/uss/uss_vol.c
@@ -61,7 +61,7 @@ extern int line;
  * will work and we can avoid nasty little core dumps.
  */
 struct ubik_client *uconn_vldbP;	/*Ubik connection struct */
-struct ubik_client *cstruct;	/*Required name for above */
+extern struct ubik_client *cstruct;	/*Required name for above */
 
 /*
  * ------------------------ Private globals -----------------------
@@ -354,44 +354,6 @@ uss_vol_GetServer(char *a_name)
     return (addr);
 
 }				/*uss_vol_GetServer */
-
-/* XXX - This function is unused, and could be deleted */
-#if 0
-/*------------------------------------------------------------------------
- * static GetVolumeType
- *
- * Description:
- *	Translate the given char string representing a volume type to the
- *	numeric representation.
- *
- * Arguments:
- *	a_type : Char string volume type.
- *
- * Returns:
- *	One of ROVOL, RWVOL, BACKVOL, or -1 on failure.
- *
- * Environment:
- *	Nothing interesting.
- *
- * Side Effects:
- *	As advertised.
- *------------------------------------------------------------------------*/
-
-static afs_int32
-GetVolumeType(char *a_type)
-{				/*GetVolumeType */
-
-    if (!strcmp(a_type, "ro"))
-	return (ROVOL);
-    else if (!strcmp(a_type, "rw"))
-	return (RWVOL);
-    else if (!strcmp(a_type, "bk"))
-	return (BACKVOL);
-    else
-	return (-1);
-
-}				/*GetVolumeType */
-#endif
 
 /*------------------------------------------------------------------------
  * EXPORTED uss_Vol_GetPartitionID
