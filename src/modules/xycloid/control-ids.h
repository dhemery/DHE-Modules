#pragma once

namespace dhe {
namespace xycloid {

struct ParamIds {
  enum {
    RatioKnob,
    RatioAvKnob,
    DirectionSwitch,
    DepthKnob,
    DepthAvKnob,
    SpeedKnob,
    SpeedAvKnob,
    XGainKnob,
    YGainKnob,
    XRangeSwitch,
    YRangeSwitch,
    FreeRatioSwitch,
    PhaseOffsetKnob,
    PhaseOffsetAvKnob,
    ParameterCount
  };
};

struct InputIds {
  enum {
    RatioCvInput,
    DepthCvInput,
    SpeedCvInput,
    XGainCvInput,
    YGainCvInput,
    PhaseCvInput,
    InputCount
  };
};

struct OutputIds {
  enum { XOutput, YOutput, OutputCount };
};
} // namespace xycloid
} // namespace dhe
