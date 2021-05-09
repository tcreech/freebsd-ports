--- src/WINNT/netidmgr_plugin/afsplugin.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/netidmgr_plugin/afsplugin.c
@@ -713,11 +713,6 @@ afs_msg_system(khm_int32 msg_subtype,
         if(afs_credtype_id >= 0) {
             kcdb_credtype_unregister(afs_credtype_id);
         }
-#if 0
-        if(afs_attr_client >= 0) {
-            kcdb_attrib_unregister(afs_attr_client);
-        }
-#endif
         if(afs_attr_cell >= 0) {
             kcdb_attrib_unregister(afs_attr_cell);
         }
@@ -771,12 +766,6 @@ afs_msg_kcdb(khm_int32 msg_subtype,
              void * vparam)
 {
     khm_int32 rv = KHM_ERROR_SUCCESS;
-
-    /* we don't really do anything with this yet */
-#if 0
-    switch(msg_subtype) {
-    }
-#endif
 
     return rv;
 }
