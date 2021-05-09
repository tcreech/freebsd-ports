--- src/platform/IRIX/rcmd.c.orig	2021-01-14 21:08:41 UTC
+++ src/platform/IRIX/rcmd.c
@@ -145,11 +145,7 @@ rcmd(ahost, rport, locuser, remuser, cmd, fd2p)
 #endif /* !defined(AIX) */
 #endif
 	sin.sin_family = hp->h_addrtype;
-#ifdef	AFS_OSF_ENV
-	memcpy((caddr_t) & sin.sin_addr, hp->h_addr_list[0], hp->h_length);
-#else
 	memcpy((caddr_t) & sin.sin_addr, hp->h_addr, hp->h_length);
-#endif
 	sin.sin_port = rport;
 	/* attempt to remote authenticate first... */
 	sp = getservbyport((int)rport, "tcp");
@@ -290,16 +286,6 @@ rcmd(ahost, rport, locuser, remuser, cmd, fd2p)
 	goto bad2;
     }
     if (c != 0) {
-#ifdef	AFS_OSF_ENV
-	/*
-	 *   Two different protocols seem to be used;
-	 *   one prepends a "message" byte with a "small"
-	 *   number; the other one just sends the message
-	 */
-	if (isalnum(c))
-	    (void)write(2, &c, 1);
-
-#endif
 	while (read(s, &c, 1) == 1) {
 	    (void)write(2, &c, 1);
 	    if (c == '\n')
@@ -387,14 +373,6 @@ ruserok(rhost, superuser, ruser, luser)
     if (hostf) {
 	if (!_validuser(hostf, fhost, luser, ruser, baselen)) {
 	    (void)fclose(hostf);
-#ifdef	AFS_OSF_ENV
-	    if (first == 0) {
-		(void)seteuid(suid);
-		(void)setegid(sgid);
-		if (group_list_size >= 0)
-		    (void)setgroups(group_list_size, groups);
-	    }
-#endif
 	    return (0);
 	}
 	(void)fclose(hostf);
