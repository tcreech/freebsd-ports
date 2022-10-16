--- client/controller.cpp.orig	2021-12-22 17:40:36 UTC
+++ client/controller.cpp
@@ -31,7 +31,6 @@
 #if defined(HAS_OPUS)
 #include "decoder/opus_decoder.hpp"
 #endif
-
 #ifdef HAS_ALSA
 #include "player/alsa_player.hpp"
 #endif
@@ -50,6 +49,9 @@
 #ifdef HAS_WASAPI
 #include "player/wasapi_player.hpp"
 #endif
+#if defined(HAS_SNDIO)
+#include "player/sndio_player.hpp"
+#endif
 #include "player/file_player.hpp"
 
 #include "browseZeroConf/browse_mdns.hpp"
@@ -110,6 +112,9 @@ std::vector<std::string> Controller::getSupportedPlaye
 #ifdef HAS_WASAPI
     result.emplace_back(player::WASAPI);
 #endif
+#ifdef HAS_SNDIO
+    result.emplace_back(player::SNDIO);
+#endif
     result.emplace_back(player::FILE);
     return result;
 }
@@ -211,6 +216,10 @@ void Controller::getNextMessage()
 #ifdef HAS_WASAPI
             if (!player_)
                 player_ = createPlayer<WASAPIPlayer>(settings_.player, player::WASAPI);
+#endif
+#ifdef HAS_SNDIO
+            if (!player_)
+                player_ = createPlayer<SndioPlayer>(settings_.player, player::SNDIO);
 #endif
             if (!player_ && (settings_.player.player_name == player::FILE))
                 player_ = createPlayer<FilePlayer>(settings_.player, player::FILE);
