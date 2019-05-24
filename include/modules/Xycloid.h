#pragma once

#include "Module.h"
#include "modules/components/Rotor.h"
#include "util/range.h"

namespace dhe {

class Xycloid : public Module {
public:
  Xycloid();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    WobbleRatioKnob,
    WobbleRatioAvKnob,
    WobbleRatioRangeSwitch,
    WobbleDepthKnob,
    WobbleDepthAvKnob,
    ThrobSpeedKnob,
    ThrobSpeedAvKnob,
    XGainKnob,
    YGainKnob,
    XRangeSwitch,
    YRangeSwitch,
    WobbleFreedomSwitch,
    WobblePhaseKnob,
    ParameterCount
  };
  enum InputIds { WobbleRatioCvInput, WobbleDepthCvInput, ThrobSpeedCvInput, XGainCvInput, YGainCvInput, InputCount };
  enum OutputIds { XOutput, YOutput, OutputCount };

private:
  auto wobbleRatioIsFree() -> bool;
  auto offset(int param) -> float;
  auto throbSpeed(float sampleTime) -> float;
  auto wobbleDepth() -> float;
  auto wobblePhase() -> float;
  auto wobbleRatioRange() -> const Range &;
  auto wobbleRatio() -> float;
  auto xOffset() -> float;
  auto xGain() -> float;
  auto yGain() -> float;
  auto yOffset() -> float;

  static constexpr auto throb_speed_knob_range = Range{-1.f, 1.f};
  static constexpr auto wobble_depth_range = Range{0.f, 1.f};

  float wobble_ratio_offset{0.f};
  Rotor wobbler{};
  Rotor throbber{};
};
} // namespace dhe
