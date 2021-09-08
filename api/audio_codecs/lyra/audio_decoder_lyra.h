/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_AUDIO_CODECS_LYRA_AUDIO_DECODER_LYRA_H_
#define API_AUDIO_CODECS_LYRA_AUDIO_DECODER_LYRA_H_

#include <memory>
#include <vector>

#include "absl/types/optional.h"
#include "api/audio_codecs/audio_codec_pair_id.h"
#include "api/audio_codecs/audio_decoder.h"
#include "api/audio_codecs/audio_format.h"
#include "rtc_base/system/rtc_export.h"
#include "api/audio_codecs/lyra/audio_encoder_lyra.h"

namespace webrtc {

// LYRA decoder API for use as a template parameter to
// CreateAudioDecoderFactory<...>().
struct RTC_EXPORT AudioDecoderLyra {
  struct Config {
    bool IsOk() const {
      return num_channels >= 1 && model_path.size() > 0;
    }
    int num_channels = 1;
/*
#if defined(WEBRTC_IOS)
    std::string model_path = "/private/var/containers/Bundle/Application/C23A2C4E-8913-4ED4-860E-C94B6C1F65B0/AppRTCMobile.app";
#else
    std::string model_path = "/storage/emulated/0/Android/data/org.appspot.apprtc/files";
#endif
*/
    std::string model_path = g_lyra_weight_path;
  };
  static absl::optional<Config> SdpToConfig(const SdpAudioFormat& audio_format);
  static void AppendSupportedDecoders(std::vector<AudioCodecSpec>* specs);
  static std::unique_ptr<AudioDecoder> MakeAudioDecoder(
      const Config& config,
      absl::optional<AudioCodecPairId> codec_pair_id = absl::nullopt);
};

}  // namespace webrtc

#endif  // API_AUDIO_CODECS_LYRA_AUDIO_DECODER_LYRA_H_
