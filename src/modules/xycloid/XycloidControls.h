#pragma once

namespace dhe {
namespace xycloid {
class XycloidControls {
public:
  enum ParameterIds {
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
  enum InputIds {
    RatioCvInput,
    DepthCvInput,
    SpeedCvInput,
    XGainCvInput,
    YGainCvInput,
    PhaseCvInput,
    InputCount
  };
  enum OutputIds { XOutput, YOutput, OutputCount };
};
} // namespace xycloid
} // namespace dhe