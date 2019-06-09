#pragma once

#include "modules/components/Range.h"
#include "modules/components/Rotor.h"
#include "modules/components/Taper.h"
#include "modules/controls/CommonInputs.h"

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
  auto xGain() const -> float { return scaledRotation(this, XGainKnob, XGainCvInput, gainRange); }

  auto xOffset() const -> float { return buttonIsPressed(this, XRangeSwitch) ? 1.F : 0.F; }

  auto yGain() const -> float { return scaledRotation(this, YGainKnob, YGainCvInput, gainRange); }

  auto yOffset() const -> float { return buttonIsPressed(this, YRangeSwitch) ? 1.F : 0.F; }

  auto speed() const -> float {
    return taperedAndScaledRotation(this, SpeedKnob, SpeedCvInput, SpeedAvKnob, speedKnobTaper, speedRange);
  }

  auto depth() const -> float;
  auto phase() const -> float;
  auto ratio() const -> float;

  Rotor wobbler{};
  Rotor throbber{};
};
} // namespace dhe
