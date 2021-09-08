/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_CODING_CODECS_LYRA_AUDIO_DECODER_LYRA_H_
#define MODULES_AUDIO_CODING_CODECS_LYRA_AUDIO_DECODER_LYRA_H_

#include <stddef.h>
#include <stdint.h>

#include <vector>

#include "api/audio_codecs/audio_decoder.h"
#include "rtc_base/buffer.h"
#include "rtc_base/checks.h"
#include "rtc_base/constructor_magic.h"

#include "lyra_decoder.h"

namespace webrtc {

class AudioDecoderLyraImpl final : public AudioDecoder {
 public:
  static const int kDefaultSampleRateHz = 16000; //chromemedia::codec::kInternalSampleRateHz;

  struct Config {
    public:
    explicit Config()
      : frame_size_ms(40), sample_rate_hz(kDefaultSampleRateHz), num_channels(1), bitrate(3000) {}

    bool IsOk() const;

    int frame_size_ms;
    int sample_rate_hz;
    size_t num_channels;
    int bitrate;
    std::string model_path;
  };

  AudioDecoderLyraImpl(const Config &config);
  ~AudioDecoderLyraImpl() override;

  void Reset() override;
  //std::vector<ParseResult> ParsePayload(rtc::Buffer&& payload,
  //                                      uint32_t timestamp) override;
  int PacketDuration(const uint8_t* encoded, size_t encoded_len) const override;
  int SampleRateHz() const override;
  size_t Channels() const override;

 protected:
  int DecodeInternal(const uint8_t* encoded,
                     size_t encoded_len,
                     int sample_rate_hz,
                     int16_t* decoded,
                     SpeechType* speech_type) override;

 private:
  const int frame_size_ms_;
  const int sample_rate_hz_;
  const size_t num_channels_;
  const int bitrate_;
  const std::string model_path_;
  std::unique_ptr<chromemedia::codec::LyraDecoder> lyra_decoder_;
  RTC_DISALLOW_COPY_AND_ASSIGN(AudioDecoderLyraImpl);
};

}  // namespace webrtc

#endif  // MODULES_AUDIO_CODING_CODECS_LYRA_AUDIO_DECODER_LYRA_H_
