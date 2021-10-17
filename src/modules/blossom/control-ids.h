#pragma once

namespace dhe {
namespace blossom {
struct Params {
  enum {
    SpinSpeed,
    SpinSpeedAv,
    BounceRatio,
    BounceRatioAv,
    BounceRatioMode,
    Depth,
    DepthAv,
    Phase,
    PhaseAv,
    XGain,
    YGain,
    XRange,
    YRange,
    N
  };
};

struct Inputs {
  enum { SpinSpeedCv, BounceRatioCv, DepthCv, PhaseCv, XGainCv, YGainCv, N };
};

struct Outputs {
  enum { XOutput, YOutput, N };
};

} // namespace blossom
} // namespace dhe
