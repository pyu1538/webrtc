/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/audio_codecs/lyra/audio_encoder_lyra.h"

#include <memory>
#include <vector>

#include "absl/strings/match.h"
#include "modules/audio_coding/codecs/lyra/audio_encoder_lyra.h"
#include "rtc_base/numerics/safe_conversions.h"
#include "rtc_base/numerics/safe_minmax.h"
#include "rtc_base/string_to_number.h"

namespace webrtc {

std::string g_lyra_weight_path = "";

absl::optional<AudioEncoderLyra::Config> AudioEncoderLyra::SdpToConfig(
    const SdpAudioFormat& format) {
  if (format.clockrate_hz == AudioEncoderLyraImpl::kDefaultSampleRateHz && format.num_channels >= 1) {
    Config config;
    config.num_channels = 1;
    config.frame_size_ms = 40;
    config.bitrate = 3000;
    config.dtx = false;
    RTC_DCHECK(config.IsOk());
    return config;
  } else {
    return absl::nullopt;
  }
}

void AudioEncoderLyra::AppendSupportedEncoders(
    std::vector<AudioCodecSpec>* specs) {
  const SdpAudioFormat fmt = {"lyra",
                              AudioEncoderLyraImpl::kDefaultSampleRateHz,
                              1,
                              {{"minptime", "40"}, {"usedtx", "0"}}};
  const AudioCodecInfo info = QueryAudioEncoder(*SdpToConfig(fmt));
  specs->push_back({fmt, info});
}

AudioCodecInfo AudioEncoderLyra::QueryAudioEncoder(const Config& config) {
  RTC_DCHECK(config.IsOk());
  return {AudioEncoderLyraImpl::kDefaultSampleRateHz, 1, 3000};
}

std::unique_ptr<AudioEncoder> AudioEncoderLyra::MakeAudioEncoder(
    const Config& config,
    int payload_type,
    absl::optional<AudioCodecPairId> /*codec_pair_id*/) {
  RTC_DCHECK(config.IsOk());
  AudioEncoderLyraImpl::Config impl_config;
  impl_config.num_channels = config.num_channels;
  impl_config.frame_size_ms = config.frame_size_ms;
  impl_config.payload_type = payload_type;
  impl_config.bitrate = config.bitrate;
  impl_config.dtx = config.dtx;
  impl_config.model_path = config.model_path;
  return std::make_unique<AudioEncoderLyraImpl>(impl_config);
}

}  // namespace webrtc
