--- src/3rdparty/chromium/ui/gl/gl_context.cc.orig	2023-12-12 22:08:45 UTC
+++ src/3rdparty/chromium/ui/gl/gl_context.cc
@@ -445,7 +445,7 @@ bool GLContext::MakeVirtuallyCurrent(
   DCHECK(virtual_context->IsCurrent(surface));
 
   if (switched_real_contexts || virtual_context != current_virtual_context_) {
-#if DCHECK_IS_ON()
+#if DCHECK_IS_ON() && !BUILDFLAG(IS_BSD)
     GLenum error = glGetError();
     // Accepting a context loss error here enables using debug mode to work on
     // context loss handling in virtual context mode.
