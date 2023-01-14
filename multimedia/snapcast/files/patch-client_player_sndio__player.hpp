--- client/player/sndio_player.hpp.orig	2022-08-03 05:33:40 UTC
+++ client/player/sndio_player.hpp
@@ -0,0 +1,64 @@
+/***
+    This file is part of snapcast
+    Copyright (C) 2014-2020  Johannes Pohl
+
+    This program is free software: you can redistribute it and/or modify
+    it under the terms of the GNU General Public License as published by
+    the Free Software Foundation, either version 3 of the License, or
+    (at your option) any later version.
+
+    This program is distributed in the hope that it will be useful,
+    but WITHOUT ANY WARRANTY; without even the implied warranty of
+    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
+    GNU General Public License for more details.
+
+    You should have received a copy of the GNU General Public License
+    along with this program.  If not, see <http://www.gnu.org/licenses/>.
+***/
+
+#ifndef SNDIO_PLAYER_HPP
+#define SNDIO_PLAYER_HPP
+
+#include <sndio.h>
+
+#include <boost/asio/steady_timer.hpp>
+
+#include "player.hpp"
+
+namespace player
+{
+
+static constexpr auto SNDIO = "sndio";
+
+/// sndio Audio Player
+/**
+ * Player implementation for sndio
+ */
+class SndioPlayer : public Player
+{
+public:
+    SndioPlayer(boost::asio::io_context& io_context, const ClientSettings::Player& settings, std::shared_ptr<Stream> stream);
+    virtual ~SndioPlayer();
+
+    void start() override;
+    void stop() override;
+
+protected:
+    double getCurrentOutputLatencyMillis() const;
+
+    void requestAudio();
+    void loop();
+    int openStream();
+    bool needsThread() const override;
+    struct sio_hdl* hdl;
+    std::vector<char> buffer_;
+    long long _realpos;
+    long long _writepos;
+
+    boost::asio::steady_timer timer_;
+    std::chrono::time_point<std::chrono::steady_clock> next_request_;
+};
+
+} // namespace player
+
+#endif
