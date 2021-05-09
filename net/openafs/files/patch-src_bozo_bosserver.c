--- src/bozo/bosserver.c.orig	2021-01-14 21:08:41 UTC
+++ src/bozo/bosserver.c
@@ -45,6 +45,7 @@
 #include <afs/fileutil.h>
 #include <afs/audit.h>
 #include <afs/cellconfig.h>
+#include <afs/cmd.h>
 
 #if defined(AFS_SGI_ENV)
 #include <afs/afs_args.h>
@@ -67,10 +68,10 @@ static int bozo_argc = 0;
 static char** bozo_argv = NULL;
 #endif
 
-const char *DoCore;
+char *DoCore;
 int DoLogging = 0;
 int DoSyslog = 0;
-const char *DoPidFiles = NULL;
+char *DoPidFiles = NULL;
 #ifndef AFS_NT40_ENV
 int DoSyslogFacility = LOG_DAEMON;
 #endif
@@ -842,9 +843,31 @@ CreateLocalCellConfig(void)
 
 #include "AFS_component_version_number.c"
 
+enum optionsList {
+    OPT_noauth,
+    OPT_log,
+    OPT_restricted,
+    OPT_pidfiles,
+    OPT_auditinterface,
+    OPT_auditlog,
+    OPT_transarc_logs,
+    OPT_peer_stats,
+    OPT_process_stats,
+    OPT_rxbind,
+    OPT_rxmaxmtu,
+    OPT_dotted,
+#ifndef AFS_NT40_ENV
+    OPT_nofork,
+    OPT_cores,
+    OPT_syslog
+#endif /* AFS_NT40_ENV */
+};
+
 int
 main(int argc, char **argv, char **envp)
 {
+    struct cmd_syndesc *opts;
+
     struct rx_service *tservice;
     afs_int32 code;
     struct afsconf_dir *tdir;
@@ -853,6 +876,7 @@ main(int argc, char **argv, char **envp)
     char *oldlog;
     int rxMaxMTU = -1;
     afs_uint32 host = htonl(INADDR_ANY);
+    char *auditIface = NULL;
     char *auditFileName = NULL;
     struct rx_securityClass **securityClasses;
     afs_int32 numClasses;
@@ -907,7 +931,11 @@ main(int argc, char **argv, char **envp)
 
     /* some path inits */
     bozo_fileName = AFSDIR_SERVER_BOZCONF_FILEPATH;
-    DoCore = AFSDIR_SERVER_LOGS_DIRPATH;
+    DoCore = strdup(AFSDIR_SERVER_LOGS_DIRPATH);
+    if (!DoCore) {
+	fprintf(stderr, "bosserver: Failed to allocate memory.\n");
+	exit(1);
+    }
 
     /* initialize the list of dirpaths that the bosserver has
      * an interest in monitoring */
@@ -931,117 +959,119 @@ main(int argc, char **argv, char **envp)
     }
     bozo_argv[0] = (char*)AFSDIR_SERVER_BOSVR_FILEPATH; /* expected path */
     bozo_argv[bozo_argc] = NULL; /* null terminate list */
+    for (i = 1; i < argc; i++) {
+	bozo_argv[i] = argv[i];
+    }
 #endif	/* AFS_NT40_ENV */
 
     /* parse cmd line */
-    for (code = 1; code < argc; code++) {
+    opts = cmd_CreateSyntax(NULL, NULL, NULL, 0, NULL);
+
+    /* bosserver specific options */
+    cmd_AddParmAtOffset(opts, OPT_noauth, "-noauth", CMD_FLAG,
+			CMD_OPTIONAL, "disable authentication");
+    cmd_AddParmAtOffset(opts, OPT_log, "-log", CMD_FLAG,
+			CMD_OPTIONAL, "enable logging of privileged commands");
+    cmd_AddParmAtOffset(opts, OPT_restricted, "-restricted", CMD_FLAG,
+			CMD_OPTIONAL, "enable restricted mode");
+    cmd_AddParmAtOffset(opts, OPT_pidfiles, "-pidfiles", CMD_SINGLE_OR_FLAG,
+			CMD_OPTIONAL, "enable creating pid files");
 #ifndef AFS_NT40_ENV
-	bozo_argv[code] = argv[code];
-#endif	/* AFS_NT40_ENV */
-	if (strcmp(argv[code], "-noauth") == 0) {
-	    /* set noauth flag */
-	    noAuth = 1;
-	} else if (strcmp(argv[code], "-log") == 0) {
-	    /* set extra logging flag */
-	    DoLogging = 1;
-	}
+    cmd_AddParmAtOffset(opts, OPT_nofork, "-nofork", CMD_FLAG,
+			CMD_OPTIONAL, "run in the foreground");
+    cmd_AddParmAtOffset(opts, OPT_cores, "-cores", CMD_SINGLE,
+			CMD_OPTIONAL, "none | path for core files");
+#endif /* AFS_NT40_ENV */
+
+    /* general server options */
+    cmd_AddParmAtOffset(opts, OPT_auditinterface, "-audit-interface", CMD_SINGLE,
+			CMD_OPTIONAL, "audit interface (file or sysvmq)");
+    cmd_AddParmAtOffset(opts, OPT_auditlog, "-auditlog", CMD_SINGLE,
+			CMD_OPTIONAL, "audit log path");
+    cmd_AddParmAtOffset(opts, OPT_transarc_logs, "-transarc-logs", CMD_FLAG,
+			CMD_OPTIONAL, "enable Transarc style logging");
+
 #ifndef AFS_NT40_ENV
-	else if (strcmp(argv[code], "-syslog") == 0) {
-	    /* set syslog logging flag */
-	    DoSyslog = 1;
-	} else if (strncmp(argv[code], "-syslog=", 8) == 0) {
-	    DoSyslog = 1;
-	    DoSyslogFacility = atoi(argv[code] + 8);
-	} else if (strncmp(argv[code], "-cores=", 7) == 0) {
-	    if (strcmp((argv[code]+7), "none") == 0)
-		DoCore = 0;
-	    else
-		DoCore = (argv[code]+7);
-	} else if (strcmp(argv[code], "-nofork") == 0) {
-	    nofork = 1;
-	}
+    cmd_AddParmAtOffset(opts, OPT_syslog, "-syslog", CMD_SINGLE_OR_FLAG,
+			CMD_OPTIONAL, "log to syslog");
 #endif
-	else if (strcmp(argv[code], "-enable_peer_stats") == 0) {
-	    DoPeerRPCStats = 1;
-	} else if (strcmp(argv[code], "-enable_process_stats") == 0) {
-	    DoProcessRPCStats = 1;
-	}
-	else if (strcmp(argv[code], "-restricted") == 0) {
-	    bozo_isrestricted = 1;
-	}
-	else if (strcmp(argv[code], "-rxbind") == 0) {
-	    rxBind = 1;
-	}
-	else if (strcmp(argv[code], "-allow-dotted-principals") == 0) {
-	    rxkadDisableDotCheck = 1;
-	}
-	else if (!strcmp(argv[code], "-rxmaxmtu")) {
-	    if ((code + 1) >= argc) {
-		fprintf(stderr, "missing argument for -rxmaxmtu\n");
-		exit(1);
-	    }
-	    rxMaxMTU = atoi(argv[++code]);
-	}
-	else if (strcmp(argv[code], "-auditlog") == 0) {
-	    auditFileName = argv[++code];
 
-	} else if (strcmp(argv[code], "-audit-interface") == 0) {
-	    char *interface = argv[++code];
+    /* rx options */
+    cmd_AddParmAtOffset(opts, OPT_peer_stats, "-enable_peer_stats", CMD_FLAG,
+			CMD_OPTIONAL, "enable RX RPC statistics by peer");
+    cmd_AddParmAtOffset(opts, OPT_process_stats, "-enable_process_stats", CMD_FLAG,
+			CMD_OPTIONAL, "enable RX RPC statistics");
+    cmd_AddParmAtOffset(opts, OPT_rxbind, "-rxbind", CMD_FLAG,
+			CMD_OPTIONAL, "bind only to the primary interface");
+    cmd_AddParmAtOffset(opts, OPT_rxmaxmtu, "-rxmaxmtu", CMD_SINGLE,
+			CMD_OPTIONAL, "maximum MTU for RX");
+    /* rxkad options */
+    cmd_AddParmAtOffset(opts, OPT_dotted, "-allow-dotted-principals", CMD_FLAG,
+			CMD_OPTIONAL, "permit Kerberos 5 principals with dots");
 
-	    if (osi_audit_interface(interface)) {
-		printf("Invalid audit interface '%s'\n", interface);
+    code = cmd_Parse(argc, argv, &opts);
+    if (code == CMD_HELP) {
+	exit(0);
+    }
+    if (code)
+	exit(1);
+
+    /* bosserver options */
+    cmd_OptionAsFlag(opts, OPT_noauth, &noAuth);
+    cmd_OptionAsFlag(opts, OPT_log, &DoLogging);
+    cmd_OptionAsFlag(opts, OPT_restricted, &bozo_isrestricted);
+
+    if (cmd_OptionPresent(opts, OPT_pidfiles)) {
+	if (cmd_OptionAsString(opts, OPT_pidfiles, &DoPidFiles) != 0) {
+	    DoPidFiles = strdup(AFSDIR_LOCAL_DIR);
+	    if (!DoPidFiles) {
+		fprintf(stderr, "bosserver: Failed to allocate memory\n");
 		exit(1);
 	    }
-	} else if (strncmp(argv[code], "-pidfiles=", 10) == 0) {
-	    DoPidFiles = (argv[code]+10);
-	} else if (strncmp(argv[code], "-pidfiles", 9) == 0) {
-	    DoPidFiles = AFSDIR_LOCAL_DIR;
-	} else if (strcmp(argv[code], "-transarc-logs") == 0) {
-	    DoTransarcLogs = 1;
 	}
-	else {
+    }
 
-	    /* hack to support help flag */
-	    int ec;
-	    if (strcmp(argv[code], "-help") == 0 || strcmp(argv[code], "-h") == 0)
-		ec = 0; /* It is not an error to ask for usage. */
-	    else {
-		printf("Unrecognized option: %s\n", argv[code]);
-		ec = 1;
-	    }
-
 #ifndef AFS_NT40_ENV
-	    printf("Usage: bosserver [-noauth] [-log] "
-		   "[-auditlog <log path>] "
-		   "[-audit-interface <file|sysvmq> (default is file)] "
-		   "[-rxmaxmtu <bytes>] [-rxbind] [-allow-dotted-principals] "
-		   "[-syslog[=FACILITY]] "
-		   "[-restricted] "
-		   "[-enable_peer_stats] [-enable_process_stats] "
-		   "[-cores=<none|path>] \n"
-		   "[-pidfiles[=path]] "
-		   "[-transarc-logs] "
-		   "[-nofork] " "[-help]\n");
-#else
-	    printf("Usage: bosserver [-noauth] [-log] "
-		   "[-auditlog <log path>] "
-		   "[-audit-interface <file|sysvmq> (default is file)] "
-		   "[-rxmaxmtu <bytes>] [-rxbind] [-allow-dotted-principals] "
-		   "[-restricted] "
-		   "[-enable_peer_stats] [-enable_process_stats] "
-		   "[-cores=<none|path>] \n"
-		   "[-pidfiles[=path]] "
-		   "[-help]\n");
+    cmd_OptionAsFlag(opts, OPT_nofork, &nofork);
+
+    if (cmd_OptionAsString(opts, OPT_cores, &DoCore) == 0) {
+	if (strcmp(DoCore, "none") == 0) {
+	    free(DoCore);
+	    DoCore = NULL;
+	}
+    }
 #endif
-	    fflush(stdout);
 
-	    exit(ec);
+    /* general server options */
+    cmd_OptionAsString(opts, OPT_auditlog, &auditFileName);
+
+    if (cmd_OptionAsString(opts, OPT_auditinterface, &auditIface) == 0) {
+	if (osi_audit_interface(auditIface)) {
+	    printf("Invalid audit interface '%s'\n", auditIface);
+	    free(auditIface);
+	    exit(1);
 	}
+	free(auditIface);
     }
-    if (auditFileName) {
-	osi_audit_file(auditFileName);
+
+    cmd_OptionAsFlag(opts, OPT_transarc_logs, &DoTransarcLogs);
+
+#ifndef AFS_NT40_ENV
+    if (cmd_OptionPresent(opts, OPT_syslog)) {
+	DoSyslog = 1;
+	cmd_OptionAsInt(opts, OPT_syslog, &DoSyslogFacility);
     }
+#endif
 
+    /* rx options */
+    cmd_OptionAsFlag(opts, OPT_peer_stats, &DoPeerRPCStats);
+    cmd_OptionAsFlag(opts, OPT_process_stats, &DoProcessRPCStats);
+    cmd_OptionAsFlag(opts, OPT_rxbind, &rxBind);
+    cmd_OptionAsInt(opts, OPT_rxmaxmtu, &rxMaxMTU);
+
+    /* rxkad options */
+    cmd_OptionAsFlag(opts, OPT_dotted, &rxkadDisableDotCheck);
+
 #ifndef AFS_NT40_ENV
     if (geteuid() != 0) {
 	printf("bosserver: must be run as root.\n");
@@ -1107,6 +1137,9 @@ main(int argc, char **argv, char **envp)
 	       DoCore ? DoCore : AFSDIR_SERVER_LOGS_DIRPATH, errno);
 	exit(1);
     }
+
+    if (auditFileName != NULL)
+	osi_audit_file(auditFileName);
 
     /* try to read the key from the config file */
     tdir = afsconf_Open(AFSDIR_SERVER_ETC_DIRPATH);
