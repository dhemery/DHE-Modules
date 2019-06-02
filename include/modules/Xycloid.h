#pragma once

#include "modules/components/Rotor.h"
#include "modules/components/Taper.h"
#include "modules/controls/Inputs.h"
#include "util/Range.h"

#include <engine/Module.hpp>

namespace dhe {

class Xycloid : public rack::engine::Module {
public:
  Xycloid();

  void process(const ProcessArgs &args) override;

  auto wobbleRatioRange() const -> Range const * {
    return selectedRange<3>(this, WobbleDirectionSwitch, wobbleRatioRanges);
  }
  auto wobbleRatioIsFree() const -> bool { return switchPosition(this, WobbleRatioModeSwitch) == 1; }

  static const std::array<const Range *, 3> wobbleRatioRanges;
  static auto constexpr speedKnobTaperCurvature = -0.8F;
  static auto constexpr speedRange = Range{-10.F, 10.F};
  static auto constexpr speedKnobTaper = taper::FixedSTaper{speedKnobTaperCurvature};

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
  auto xGain() const -> float { return scaledRotation(this, XGainKnob, XGainCvInput, gainRange); }

  auto xOffset() const -> float { return buttonIsPressed(this, XRangeSwitch) ? 1.F : 0.F; }

  auto yGain() const -> float { return scaledRotation(this, YGainKnob, YGainCvInput, gainRange); }

  auto yOffset() const -> float { return buttonIsPressed(this, YRangeSwitch) ? 1.F : 0.F; }

  auto throbSpeed() const -> float {
    return taperedAndScaledRotation(this, ThrobSpeedKnob, ThrobSpeedCvInput, ThrobSpeedAvKnob, speedKnobTaper,
                                    speedRange);
  }

  auto wobbleDepth() const -> float;
  auto wobblePhase() const -> float;
  auto wobbleRatio() const -> float;

  Rotor wobbler{};
  Rotor throbber{};
};
} // namespace dhe
