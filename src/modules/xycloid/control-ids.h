#pragma once

namespace dhe {
namespace xycloid {

struct ParamId {
  enum {
    WobbleRatio,
    WobbleRatioAv,
    WobbleRatioRange,
    WobbleDepth,
    WobbleDepthAv,
    ThrobSpeed,
    ThrobSpeedAv,
    XGain,
    YGain,
    XRange,
    YRange,
    WobbleRatioMode,
    WobblePhaseOffset,
    WobblePhaseOffsetAv,
    Count
  };
};

struct InputId {
  enum {
    WobbleRatioCv,
    WobbleDepthCv,
    ThrobSpeedCv,
    XGainCv,
    YGainCv,
    WobblePhaseOffsetCv,
    Count
  };
};

struct OutputId {
  enum { X, Y, Count };
};
} // namespace xycloid
} // namespace dhe
