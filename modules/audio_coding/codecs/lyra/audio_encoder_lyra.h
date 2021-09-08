/*
 *  Copyright (c) 2014 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_CODING_CODECS_LYRA_AUDIO_ENCODER_LYRA_H_
#define MODULES_AUDIO_CODING_CODECS_LYRA_AUDIO_ENCODER_LYRA_H_

#include <utility>
#include <vector>

#include "absl/types/optional.h"
#include "api/audio_codecs/audio_encoder.h"
#include "api/units/time_delta.h"
#include "rtc_base/constructor_magic.h"

#include "lyra_encoder.h"

namespace webrtc {

class AudioEncoderLyraImpl : public AudioEncoder {
 public:
  static const int kDefaultSampleRateHz = 16000; //chromemedia::codec::kInternalSampleRateHz;
  static const int kDefaultPayloadType = 125;

  struct Config {
   public:
     explicit Config(int pt = kDefaultPayloadType)
      : frame_size_ms(40), num_channels(1), payload_type(pt), bitrate(3000), dtx(false) {}

    bool IsOk() const;

    int frame_size_ms;
    size_t num_channels;
    int payload_type;
    int bitrate;
    bool dtx;
    std::string model_path;
  };

  AudioEncoderLyraImpl(const Config& config, int sample_rate_hz = kDefaultSampleRateHz);
  ~AudioEncoderLyraImpl() override;

  int SampleRateHz() const override;
  size_t NumChannels() const override;
  size_t Num10MsFramesInNextPacket() const override;
  size_t Max10MsFramesInAPacket() const override;
  int GetTargetBitrate() const override;
  void Reset() override;
  absl::optional<std::pair<TimeDelta, TimeDelta>> GetFrameLengthRange()
      const override;

 protected:
  EncodedInfo EncodeImpl(uint32_t rtp_timestamp,
                         rtc::ArrayView<const int16_t> audio,
                         rtc::Buffer* encoded) override;

  virtual size_t EncodeCall(const int16_t* audio,
                            size_t input_len,
                            uint8_t* encoded);

  virtual size_t BytesPerSample() const;

  // Used to set EncodedInfoLeaf::encoder_type in
  // AudioEncoderPcm::EncodeImpl
  virtual AudioEncoder::CodecType GetCodecType() const;

 private:
  const int sample_rate_hz_;
  const size_t num_channels_;
  const int payload_type_;
  const int bitrate_;
  const bool dtx_;
  const size_t num_10ms_frames_per_packet_;
  const size_t full_frame_samples_;
  const std::string model_path_;
  std::vector<int16_t> speech_buffer_;
  uint32_t first_timestamp_in_buffer_;
  std::unique_ptr<chromemedia::codec::LyraEncoder> lyra_encoder_;
};

}  // namespace webrtc

#endif  // MODULES_AUDIO_CODING_CODECS_LYRA_AUDIO_ENCODER_LYRA_H_
