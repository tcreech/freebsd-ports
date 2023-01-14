--- client/player/sndio_player.cpp.orig	2022-08-03 05:33:40 UTC
+++ client/player/sndio_player.cpp
@@ -0,0 +1,190 @@
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
+#include <assert.h>
+#include <iostream>
+
+#include "common/aixlog.hpp"
+#include "common/snap_exception.hpp"
+#include "common/str_compat.hpp"
+#include "sndio_player.hpp"
+
+using namespace std;
+
+namespace player
+{
+
+static constexpr auto LOG_TAG = "SndioPlayer";
+static constexpr auto kDefaultBuffer = 50ms;
+
+
+int SndioPlayer::openStream()
+{
+    auto numFrames = static_cast<uint32_t>(stream_->getFormat().msRate() * kDefaultBuffer.count());
+    auto needed = numFrames * stream_->getFormat().frameSize();
+
+    int bufsize = needed;
+    unsigned int samplerate = stream_->getFormat().rate();
+    unsigned int channels = stream_->getFormat().channels();
+    unsigned int *pchannels = &channels;
+	unsigned int bits = stream_->getFormat().bits();
+
+    /* Do we assume signed and little endian? */
+	unsigned int sig = 1;
+	unsigned int le  = 1;
+
+    this->hdl = sio_open(SIO_DEVANY, SIO_PLAY, 0);
+    if (hdl == 0) {
+        LOG(INFO, LOG_TAG) << "sndio: failed to open device\n";
+        return (-1);
+    }
+
+    struct sio_par par;
+    sio_initpar(&par);
+	par.pchan = *pchannels;
+	par.sig = sig;
+	par.bits = bits;
+	par.bps = SIO_BPS(bits);
+	par.le = le;
+	par.rate = samplerate;
+	par.appbufsz = bufsize;
+	par.xrun = SIO_SYNC;
+	if (!sio_setpar(hdl, &par)) {
+		LOG(ERROR, LOG_TAG) << "internal error, sio_setpar() failed\n";
+        return -1;
+    }
+	if (!sio_getpar(hdl, &par)) {
+		LOG(ERROR, LOG_TAG) << "internal error, sio_getpar() failed\n";
+        return -1;
+    }
+	if (par.pchan != *pchannels) {
+		LOG(ERROR, LOG_TAG) << "couldn't set number of channels\n";
+        return -1;
+    }
+	if (par.sig != sig || par.bits != bits || par.le != le) {
+		LOG(ERROR, LOG_TAG) << "couldn't set format\n";
+        return -1;
+    }
+	if (par.bits != bits) {
+		LOG(ERROR, LOG_TAG) << "couldn't set precision\n";
+        return -1;
+    }
+	if (par.rate < samplerate * 995 / 1000 ||
+			par.rate > samplerate * 1005 / 1000) {
+		LOG(ERROR, LOG_TAG) << "couldn't set rate\n";
+        return -1;
+    }
+	if (par.xrun != SIO_SYNC) {
+		LOG(ERROR, LOG_TAG) << "couldn't set xrun policy\n";
+        return -1;
+    }
+
+    auto sndio_onmove_cb = [](void *addr, int delta) -> void {
+        SndioPlayer* p = (SndioPlayer*)addr;
+        p->_realpos += (delta * p->stream_->getFormat().frameSize());
+    };
+
+    /* Set up a callback for delay feedback */
+    sio_onmove(hdl, sndio_onmove_cb, this);
+
+	/* Start the device.*/
+	if (!sio_start(hdl)) {
+		LOG(ERROR, LOG_TAG) << "couldn't start device\n";
+        return -1;
+    }
+
+	return 0;
+}
+
+SndioPlayer::SndioPlayer(boost::asio::io_context& io_context, const ClientSettings::Player& settings, std::shared_ptr<Stream> stream)
+    : Player(io_context, settings, stream), timer_(io_context)
+{
+    auto result = openStream();
+    if (result != 0)
+        throw SnapException("Error opening sndio device");
+}
+
+SndioPlayer::~SndioPlayer()
+{
+    stop();
+}
+
+bool SndioPlayer::needsThread() const
+{
+    return false;
+}
+
+void SndioPlayer::loop()
+{
+    next_request_ += kDefaultBuffer;
+    auto now = std::chrono::steady_clock::now();
+    if (next_request_ < now)
+        next_request_ = now + 1ms;
+
+    timer_.expires_at(next_request_);
+    timer_.async_wait([this](boost::system::error_code ec) {
+        if (ec)
+            return;
+        requestAudio();
+    });
+}
+
+void SndioPlayer::start()
+{
+    next_request_ = std::chrono::steady_clock::now();
+    loop();
+}
+
+void SndioPlayer::stop()
+{
+    timer_.cancel();
+    /* TODO */
+}
+
+void SndioPlayer::requestAudio()
+{
+    auto numFrames = static_cast<uint32_t>(stream_->getFormat().msRate() * kDefaultBuffer.count());
+    auto needed = numFrames * stream_->getFormat().frameSize();
+    if (buffer_.size() < needed)
+        buffer_.resize(needed);
+
+    long long latency_bytes = _writepos - _realpos;
+    long long latency_frames = latency_bytes / stream_->getFormat().frameSize();
+    chronos::usec delay( static_cast<uint32_t>(latency_frames / stream_->getFormat().usRate()) );
+
+    if (!stream_->getPlayerChunkOrSilence(buffer_.data(), delay, numFrames))
+    {
+        LOG(INFO, LOG_TAG) << "Failed to get chunk. Playing silence.\n";
+    }
+    else
+    {
+        adjustVolume(static_cast<char*>(buffer_.data()), numFrames);
+    }
+
+    if (this->hdl)
+    {
+        auto w = sio_write(this->hdl, buffer_.data(), needed);
+        if (w >= 0)
+            _writepos += w;
+    }
+
+
+    loop();
+}
+
+} // namespace player
