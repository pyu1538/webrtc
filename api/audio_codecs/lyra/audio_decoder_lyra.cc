/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/audio_codecs/lyra/audio_decoder_lyra.h"

#include <memory>
#include <vector>

#include "absl/strings/match.h"
#include "modules/audio_coding/codecs/lyra/audio_decoder_lyra.h"
#include "rtc_base/numerics/safe_conversions.h"

namespace webrtc {

absl::optional<AudioDecoderLyra::Config> AudioDecoderLyra::SdpToConfig(
    const SdpAudioFormat& format) {
  Config config;
  config.num_channels = rtc::dchecked_cast<int>(format.num_channels);
  RTC_DCHECK(config.IsOk());
  return config;
}

void AudioDecoderLyra::AppendSupportedDecoders(
    std::vector<AudioCodecSpec>* specs) {
  const SdpAudioFormat fmt = {"lyra",
                              AudioDecoderLyraImpl::kDefaultSampleRateHz,
                              1,
                              {{"minptime", "40"}, {"usedtx", "0"}}};
  const AudioCodecInfo info{AudioDecoderLyraImpl::kDefaultSampleRateHz, 1, 3000};
  specs->push_back({fmt, info});
}

std::unique_ptr<AudioDecoder> AudioDecoderLyra::MakeAudioDecoder(
    const Config& config,
    absl::optional<AudioCodecPairId> /*codec_pair_id*/) {
  RTC_DCHECK(config.IsOk());
  AudioDecoderLyraImpl::Config impl_config;
  impl_config.bitrate = 3000;
  impl_config.frame_size_ms = 40;
  impl_config.sample_rate_hz = AudioDecoderLyraImpl::kDefaultSampleRateHz;
  impl_config.num_channels = config.num_channels;
  impl_config.model_path = config.model_path;
  return std::make_unique<AudioDecoderLyraImpl>(impl_config);
}

}  // namespace webrtc
