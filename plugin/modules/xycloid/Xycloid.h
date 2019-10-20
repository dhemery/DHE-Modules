#pragma once

#include "CommonInputs.h"
#include "LoopingPhaseAccumulator.h"
#include "Range.h"
#include "Taper.h"

#include <engine/Module.hpp>

namespace dhe {

class Xycloid : public rack::engine::Module {
public:
  Xycloid();

  void process(const ProcessArgs &args) override;

  static auto constexpr speedKnobTaperCurvature = -0.8F;
  static auto constexpr speedRange = Range{-10.F, 10.F};
  static auto constexpr speedKnobTaper = taper::FixedSTaper{speedKnobTaperCurvature};

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

  auto speed() const -> float {
    return taperedAndScaledRotation(params[SpeedKnob], inputs[SpeedCvInput], params[SpeedAvKnob], speedKnobTaper,
                                    speedRange);
  }

  auto depth() const -> float;
  auto phase() const -> float;
  auto ratio() const -> float;

  LoopingPhaseAccumulator wobbler{};
  LoopingPhaseAccumulator throbber{};
};
} // namespace dhe
