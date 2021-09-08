/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_AUDIO_CODECS_LYRA_AUDIO_ENCODER_LYRA_H_
#define API_AUDIO_CODECS_LYRA_AUDIO_ENCODER_LYRA_H_

#include <memory>
#include <vector>

#include "absl/types/optional.h"
#include "api/audio_codecs/audio_codec_pair_id.h"
#include "api/audio_codecs/audio_encoder.h"
#include "api/audio_codecs/audio_format.h"
#include "rtc_base/system/rtc_export.h"

namespace webrtc {

extern std::string g_lyra_weight_path;

// Lyra encoder API for use as a template parameter to
// CreateAudioEncoderFactory<...>().
struct RTC_EXPORT AudioEncoderLyra {
  struct Config {
    bool IsOk() const {
      return frame_size_ms > 0 && frame_size_ms % 10 == 0 && num_channels >= 1 && 3000 == bitrate && model_path.size() > 0;
    }

    int num_channels = 1;
    int frame_size_ms = 40;
    int bitrate = 3000;
    bool dtx = false;
/*
#if defined(WEBRTC_IOS)
    std::string model_path = "/private/var/containers/Bundle/Application/C23A2C4E-8913-4ED4-860E-C94B6C1F65B0/AppRTCMobile.app";
#else
    std::string model_path = "/storage/emulated/0/Android/data/org.appspot.apprtc/files";
#endif
*/
    std::string model_path = g_lyra_weight_path;
  };
  static absl::optional<AudioEncoderLyra::Config> SdpToConfig(
      const SdpAudioFormat& audio_format);
  static void AppendSupportedEncoders(std::vector<AudioCodecSpec>* specs);
  static AudioCodecInfo QueryAudioEncoder(const Config& config);
  static std::unique_ptr<AudioEncoder> MakeAudioEncoder(
      const Config& config,
      int payload_type,
      absl::optional<AudioCodecPairId> codec_pair_id = absl::nullopt);
};

}  // namespace webrtc

#endif  // API_AUDIO_CODECS_LYRA_AUDIO_ENCODER_LYRA_H_
