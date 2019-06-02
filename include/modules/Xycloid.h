#pragma once

#include "modules/components/Rotor.h"
#include "util/Range.h"

#include <engine/Module.hpp>
#include <functional>

namespace dhe {

class Xycloid : public rack::engine::Module {
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
  auto offset(int param) -> float;
  auto wobbleDepth() -> float;
  auto wobblePhase() -> float;
  auto wobbleRatio() -> float;
  auto xOffset() -> float;
  auto xGain() -> float;
  auto yGain() -> float;
  auto yOffset() -> float;

  std::function<Range const *()> wobbleRatioRange;
  std::function<bool()> wobbleRatioIsFree;
  std::function<float()> throbSpeed;
  Rotor wobbler{};
  Rotor throbber{};
};
} // namespace dhe
