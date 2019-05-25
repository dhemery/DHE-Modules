#pragma once

#include "Module.h"
#include "modules/components/Rotor.h"
#include "util/Range.h"

namespace dhe {

class Xycloid : public Module {
public:
  Xycloid();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    WobbleRatioKnob,
    WobbleRatioAvKnob,
    WobbleDirectionSwitch,
    WobbleDepthKnob,
    WobbleDepthAvKnob,
    ThrobSpeedKnob,
    ThrobSpeedAvKnob,
    XGainKnob,
    YGainKnob,
    XRangeSwitch,
    YRangeSwitch,
    WobbleRatioModeSwitch,
    WobblePhaseOffsetKnob,
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

  float wobbleRatioOffset{0.F};
  Rotor wobbler{};
  Rotor throbber{};
};
} // namespace dhe
