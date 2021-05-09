--- src/viced/host.c.orig	2021-01-14 21:08:41 UTC
+++ src/viced/host.c
@@ -67,6 +67,8 @@ int hostCount = 0;		/* number of hosts in hostList */
 int rxcon_ident_key;
 int rxcon_client_key;
 
+struct host *(hosttableptrs[h_MAXHOSTTABLES]);
+
 static struct rx_securityClass *sc = NULL;
 static int h_quota_limit;
 
@@ -675,11 +677,7 @@ h_Alloc_r(struct rx_connection *r_con)
     h_AddHostToAddrHashTable_r(host->z.host, host->z.port, host);
 
     if (consolePort == 0) {	/* find the portal number for console */
-#if	defined(AFS_OSF_ENV)
-	serverentry = getservbyname("ropcons", "");
-#else
 	serverentry = getservbyname("ropcons", 0);
-#endif
 	if (serverentry)
 	    consolePort = serverentry->s_port;
 	else
