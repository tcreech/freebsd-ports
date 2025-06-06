--- remoting/host/setup/start_host_main.cc.orig	2025-05-07 06:48:23 UTC
+++ remoting/host/setup/start_host_main.cc
@@ -45,7 +45,7 @@
 #include <unistd.h>
 #endif  // BUILDFLAG(IS_POSIX)
 
-#if BUILDFLAG(IS_LINUX)
+#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_BSD)
 #include "remoting/host/setup/daemon_controller_delegate_linux.h"
 #include "remoting/host/setup/start_host_as_root.h"
 #endif  // BUILDFLAG(IS_LINUX)
@@ -369,7 +369,7 @@ bool InitializeCloudMachineParams(HostStarter::Params&
 }  // namespace
 
 int StartHostMain(int argc, char** argv) {
-#if BUILDFLAG(IS_LINUX)
+#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_BSD)
   // Minimize the amount of code that runs as root on Posix systems.
   if (getuid() == 0) {
     return remoting::StartHostAsRoot(argc, argv);
@@ -394,7 +394,7 @@ int StartHostMain(int argc, char** argv) {
 
   mojo::core::Init();
 
-#if BUILDFLAG(IS_LINUX)
+#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_BSD)
   if (command_line->HasSwitch("no-start")) {
     // On Linux, registering the host with systemd and starting it is the only
     // reason start_host requires root. The --no-start options skips that final
