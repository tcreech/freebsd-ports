--- native/arguments.c.orig	2025-01-06 00:00:00 UTC
+++ native/arguments.c
@@ -157,7 +157,7 @@ static arg_data *parse(int argc, char *argv[])
     args->help = false;                /* Don't display help */
     args->chck = false;                /* Don't do a check-only startup */
     args->stop = false;                /* Stop a running jsvc */
-    args->wait = 0;                    /* Wait until jsvc has started the JVM */
+    args->wait = 10;                   /* Wait until jsvc has started the JVM */
     args->restarts = -1;               /* Infinite restarts by default */
     args->install = false;             /* Don't install as a service */
     args->remove = false;              /* Don't remove the installed service */
@@ -266,10 +266,6 @@ static arg_data *parse(int argc, char *argv[])
             temp = optional(argc, argv, x++);
             if (temp)
                 args->wait = atoi(temp);
-            if (args->wait < 10) {
-                log_error("Invalid wait time specified (min=10)");
-                return NULL;
-            }
         }
         else if (!strcmp(argv[x], "-restarts")) {
             temp = optional(argc, argv, x++);
@@ -424,6 +420,10 @@ static arg_data *parse(int argc, char *argv[])
         }
         /* Java 11 specific options */
         else if (!strncmp(argv[x], "--enable-preview", 16)) {
+            args->opts[args->onum++] = strdup(argv[x]);
+        }
+        /* Java 21 specific options */
+        else if (!strncmp(argv[x], "--enable-native-access=", 23)) {
             args->opts[args->onum++] = strdup(argv[x]);
         }
         else if (*argv[x] == '-') {
