/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_coding/codecs/lyra/audio_decoder_lyra.h"

#include <utility>

namespace webrtc {

AudioDecoderLyraImpl::AudioDecoderLyraImpl(const Config &config)
  : frame_size_ms_(config.frame_size_ms)
  , sample_rate_hz_(config.sample_rate_hz)
  , num_channels_(config.num_channels)
  , bitrate_(config.bitrate)
  , model_path_(config.model_path) {
  RTC_DCHECK_GE(num_channels_, 1);
  lyra_decoder_ = chromemedia::codec::LyraDecoder::Create(sample_rate_hz_, num_channels_, bitrate_, model_path_);
}

AudioDecoderLyraImpl::~AudioDecoderLyraImpl() = default;

void AudioDecoderLyraImpl::Reset() {}

int AudioDecoderLyraImpl::SampleRateHz() const {
  return sample_rate_hz_;
}

size_t AudioDecoderLyraImpl::Channels() const {
  return num_channels_;
}

int AudioDecoderLyraImpl::DecodeInternal(const uint8_t* encoded,
                                     size_t encoded_len,
                                     int sample_rate_hz,
                                     int16_t* decoded,
                                     SpeechType* speech_type) {
  if (!lyra_decoder_) return -3;
  RTC_DCHECK_EQ(SampleRateHz(), sample_rate_hz);
  int16_t temp_type = 1;  // Default is speech.
  const absl::Span<const uint8_t> encoded_packet = absl::MakeConstSpan(encoded, encoded_len);
  if (!lyra_decoder_->SetEncodedPacket(encoded_packet)) {
    //LOG(ERROR) << "Unable to set encoded packet starting at byte " << encoded_index;
    return -1;
  }

  const int num_samples_per_packet = sample_rate_hz_/1000*frame_size_ms_;
  auto decoded_or = lyra_decoder_->DecodeSamples(num_samples_per_packet);
  size_t ret = 0;
  if (!decoded_or.has_value()) {
    //LOG(ERROR) << "Unable to decode features starting at byte " << encoded_index;
    return -2;
  }

  std::vector<int16_t> &decoded_val = decoded_or.value();
  ret = decoded_val.size();
  for (size_t i = 0; i < ret; ++i) {
    decoded[i] = decoded_val[i];
  }

  *speech_type = ConvertSpeechType(temp_type);
  return static_cast<int>(ret);
}

int AudioDecoderLyraImpl::PacketDuration(const uint8_t* encoded,
                                     size_t encoded_len) const {
  // One encoded byte per sample per channel.
  return static_cast<int>(encoded_len / Channels());
}

}  // namespace webrtc
