#pragma once

namespace dhe {
namespace blossom {
  class BlossomControls {
  public:
    enum ParameterIds {
      SpeedKnob,
      SpeedAvKNob,
      RatioKnob,
      RatioAvKnob,
      FreeRatioSwitch,
      DepthKnob,
      DepthAvKnob,
      PhaseOffsetKnob,
      PhaseOffsetAvKnob,
      XGainKnob,
      YGainKnob,
      XRangeSwitch,
      YRangeSwitch,
      ParameterCount
    };
    enum InputIds { SpeedCvInput, RatioCvInput, DepthCvInput, PhaseCvInput, XGainCvInput, YGainCvInput, InputCount };
    enum OutputIds { XOutput, YOutput, OutputCount };
  };

} // namespace blossom
} // namespace dhe