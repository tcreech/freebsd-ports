--- src/3rdparty/chromium/chrome/browser/extensions/api/runtime/chrome_runtime_api_delegate.cc.orig	2023-09-29 10:14:06 UTC
+++ src/3rdparty/chromium/chrome/browser/extensions/api/runtime/chrome_runtime_api_delegate.cc
@@ -280,7 +280,9 @@ bool ChromeRuntimeAPIDelegate::GetPlatformInfo(Platfor
   } else if (strcmp(os, "linux") == 0) {
     info->os = extensions::api::runtime::PlatformOs::kLinux;
   } else if (strcmp(os, "openbsd") == 0) {
-    info->os = extensions::api::runtime::PlatformOs::kOpenbsd;
+    info->os = extensions::api::runtime::PlatformOs::kLinux;
+  } else if (strcmp(os, "freebsd") == 0) {
+    info->os = extensions::api::runtime::PlatformOs::kLinux;
   } else if (strcmp(os, "fuchsia") == 0) {
     info->os = extensions::api::runtime::PlatformOs::kFuchsia;
   } else {
