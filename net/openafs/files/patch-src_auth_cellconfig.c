--- src/auth/cellconfig.c.orig	2021-01-14 21:08:41 UTC
+++ src/auth/cellconfig.c
@@ -214,11 +214,7 @@ afsconf_FindService(const char *aname)
     if (aname == NULL || aname[0] == '\0')
 	return -1;
 
-#if     defined(AFS_OSF_ENV)
-    ts = getservbyname(aname, "");
-#else
     ts = (struct servent *) getservbyname(aname, NULL);
-#endif
     if (ts) {
 	/* we found it in /etc/services, so we use this value */
 	return ts->s_port;	/* already in network byte order */
