--- src/vfsck/fsck.h.orig	2021-01-14 21:08:41 UTC
+++ src/vfsck/fsck.h
@@ -114,13 +114,6 @@ struct bufarea *getdatablk();
 #define	sblock		(*sblk.b_un.b_fs)
 #define	cgrp		(*cgblk.b_un.b_cg)
 
-#ifdef	AFS_OSF_ENV
-/*
- * struct direct -> struct dirent
-*/
-#define	direct	dirent
-#endif /* AFS_OSF_ENV */
-
 enum fixstate { DONTKNOW, NOFIX, FIX };
 
 struct inodesc {
@@ -250,12 +243,7 @@ int rflag;			/* check raw file systems */
 #include <sys/sysmacros.h>
 FILE *logfile;			/* additional place for log message, for non-root file systems */
 #else /* AFS_SUN5_ENV */
-#ifdef	AFS_OSF_ENV
-FILE *logfile;			/* additional place for log message, for non-root file systems */
-char fflag;			/* force fsck to check a mounted fs */
-#else /* AFS_OSF_ENV */
 struct _iobuf *logfile;		/* additional place for log message, for non-root file systems */
-#endif /* AFS_OSF_ENV */
 #endif /* AFS_SUN5_ENV */
 #endif /* VICE */
 
