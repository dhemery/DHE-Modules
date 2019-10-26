#pragma once

#include "components/LoopingPhaseAccumulator.h"
#include "components/Range.h"
#include "components/Taper.h"
#include "controls/CommonInputs.h"

#include <engine/Module.hpp>

namespace dhe {

class Xycloid : public rack::engine::Module {
public:
  Xycloid();

  void process(const ProcessArgs &args) override;

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
  enum InputIds { RatioCvInput, DepthCvInput, SpeedCvInput, XGainCvInput, YGainCvInput, PhaseCvInput, InputCount };
  enum OutputIds { XOutput, YOutput, OutputCount };

private:
  auto xGain() const -> float { return gainRange.scale(rotation(params[XGainKnob], inputs[XGainCvInput])); }

  auto xOffset() const -> float { return buttonIsPressed(params[XRangeSwitch]) ? 1.F : 0.F; }

  auto yGain() const -> float { return gainRange.scale(rotation(params[YGainKnob], inputs[YGainCvInput])); }

  auto yOffset() const -> float { return buttonIsPressed(params[YRangeSwitch]) ? 1.F : 0.F; }

  auto speed() const -> float;

  auto depth() const -> float;
  auto phase() const -> float;
  auto ratio() const -> float;

  LoopingPhaseAccumulator wobbler{};
  LoopingPhaseAccumulator throbber{};
};
} // namespace dhe
