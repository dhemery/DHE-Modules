#pragma once

namespace dhe {
namespace blossom {
struct Param {
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

struct Input {
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

struct Output {
  enum { X, Y, Count };
};

} // namespace blossom
} // namespace dhe
