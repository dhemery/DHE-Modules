#pragma once

namespace dhe {
namespace xycloid {

struct ParamIds {
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

struct InputIds {
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

struct OutputIds {
  enum { X, Y, Count };
};
} // namespace xycloid
} // namespace dhe
