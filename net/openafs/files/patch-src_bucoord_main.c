--- src/bucoord/main.c.orig	2021-01-14 21:08:41 UTC
+++ src/bucoord/main.c
@@ -47,9 +47,9 @@ char tcell[64];
 /*
  * Global configuration information for the Backup Coordinator.
  */
-struct bc_config *bc_globalConfig;	/*Ptr to global BC configuration info */
+extern struct bc_config *bc_globalConfig;	/*Ptr to global BC configuration info */
 
-struct ubik_client *cstruct;	/* Ptr to Ubik client structure */
+extern struct ubik_client *cstruct;	/* Ptr to Ubik client structure */
 time_t tokenExpires;		/* The token's expiration time */
 
 static const char *DefaultConfDir;	/*Default backup config directory */
@@ -161,8 +161,6 @@ bc_InitTextConfig(void)
 {
     udbClientTextP ctPtr;
     int i;
-
-    extern struct bc_config *bc_globalConfig;
 
     mkdir(DefaultConfDir, 777);	/* temporary */
 
