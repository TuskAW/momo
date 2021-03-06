#ifndef VIDEO_CODEC_INFO_H_
#define VIDEO_CODEC_INFO_H_

#if USE_NVCODEC_ENCODER
#include "hwenc_nvcodec/nvcodec_h264_encoder.h"
#endif

struct VideoCodecInfo {
  enum class Type {
    Jetson,
    MMAL,
    NVIDIA,
    Intel,
    VideoToolbox,
    Software,
    Auto,
  };

  std::vector<Type> vp8_encoders;
  std::vector<Type> vp8_decoders;

  std::vector<Type> vp9_encoders;
  std::vector<Type> vp9_decoders;

  std::vector<Type> av1_encoders;
  std::vector<Type> av1_decoders;

  std::vector<Type> h264_encoders;
  std::vector<Type> h264_decoders;

  static std::pair<std::string, std::string> TypeToString(Type type) {
    switch (type) {
      case Type::Jetson:
        return {"Jetson", "jetson"};
      case Type::MMAL:
        return {"MMAL", "mmal"};
      case Type::NVIDIA:
        return {"NVIDIA VIDEO CODEC SDK", "nvidia"};
      case Type::Intel:
        return {"Intel Media SDK", "intel"};
      case Type::VideoToolbox:
        return {"VideoToolbox", "videotoolbox"};
      case Type::Software:
        return {"Software", "software"};
      default:
        return {"Unknown", "unknown"};
    }
  }

  static VideoCodecInfo Get() {
#if defined(_WIN32)
    return GetWindows();
#elif defined(__APPLE__)
    return GetMacos();
#elif defined(__linux__)
    return GetLinux();
#endif
  }

 private:
#if defined(_WIN32)

  static VideoCodecInfo GetWindows() {
    VideoCodecInfo info;

#if USE_NVCODEC_ENCODER
    if (NvCodecH264Encoder::IsSupported()) {
      info.h264_encoders.push_back(Type::NVIDIA);
    }
#endif

    info.vp8_encoders.push_back(Type::Software);
    info.vp8_decoders.push_back(Type::Software);
    info.vp9_encoders.push_back(Type::Software);
    info.vp9_decoders.push_back(Type::Software);
    info.av1_encoders.push_back(Type::Software);
    info.av1_decoders.push_back(Type::Software);

    return info;
  }

#elif defined(__APPLE__)

  static VideoCodecInfo GetMacos() {
    VideoCodecInfo info;

    info.h264_encoders.push_back(Type::VideoToolbox);
    info.h264_decoders.push_back(Type::VideoToolbox);
    info.vp8_encoders.push_back(Type::Software);
    info.vp9_encoders.push_back(Type::Software);
    info.vp8_decoders.push_back(Type::Software);
    info.vp9_decoders.push_back(Type::Software);
    info.av1_encoders.push_back(Type::Software);
    info.av1_decoders.push_back(Type::Software);

    return info;
  }

#elif defined(__linux__)

  static VideoCodecInfo GetLinux() {
    VideoCodecInfo info;

#if USE_NVCODEC_ENCODER
    if (NvCodecH264Encoder::IsSupported()) {
      info.h264_encoders.push_back(Type::NVIDIA);
    }
#endif

#if USE_MMAL_ENCODER
    info.h264_encoders.push_back(Type::MMAL);
    info.h264_decoders.push_back(Type::MMAL);
#endif

#if USE_JETSON_ENCODER
    info.h264_encoders.push_back(Type::Jetson);
    info.h264_decoders.push_back(Type::Jetson);
    info.vp8_decoders.push_back(Type::Jetson);
    info.vp9_decoders.push_back(Type::Jetson);
#endif

    info.vp8_encoders.push_back(Type::Software);
    info.vp8_decoders.push_back(Type::Software);
    info.vp9_encoders.push_back(Type::Software);
    info.vp9_decoders.push_back(Type::Software);

#if !defined(__arm__) || defined(__aarch64__) || defined(__ARM_NEON__)
    info.av1_encoders.push_back(Type::Software);
    info.av1_decoders.push_back(Type::Software);
#endif

    return info;
  }

#endif
};

#endif  // VIDEO_CODEC_INFO_H_
