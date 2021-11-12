#pragma once

namespace dhe {
namespace blossom {
struct ParamId {
  enum {
    SpinSpeed,
    SpinSpeedAv,
    BounceRatio,
    BounceRatioAv,
    BounceRatioMode,
    BounceDepth,
    BounceDepthAv,
    BouncePhaseOffset,
    BouncePhaseOffsetAv,
    XGain,
    YGain,
    XRange,
    YRange,
    Count
  };
};

struct InputId {
  enum {
    SpinSpeedCv,
    BounceRatioCv,
    BounceDepthCv,
    BouncePhaseOffsetCv,
    XGainCv,
    YGainCv,
    Count
  };
};

struct OutputId {
  enum { X, Y, Count };
};

} // namespace blossom
} // namespace dhe
