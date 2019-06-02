#pragma once

#include "modules/components/Rotor.h"
#include "modules/components/Taper.h"
#include "modules/controls/Controls.h"
#include "modules/controls/Inputs.h"
#include "util/Range.h"

#include <engine/Module.hpp>

namespace dhe {

class Xycloid : public rack::engine::Module {
public:
  Xycloid();

  void process(const ProcessArgs &args) override;

  auto wobbleRatioRange() -> Range const * { return selectedRange<3>(this, WobbleDirectionSwitch, wobbleRatioRanges); }
  auto wobbleRatioIsFree() -> bool { return position(this, WobbleRatioModeSwitch) == 1; }

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
  auto xGain() -> float { return scaledRotation(this, XGainKnob, XGainCvInput, gainRange); }

  auto xOffset() -> float { return buttonIsPressed(this, XRangeSwitch) ? 1.F : 0.F; }

  auto yGain() -> float { return scaledRotation(this, YGainKnob, YGainCvInput, gainRange); }

  auto yOffset() -> float { return buttonIsPressed(this, YRangeSwitch) ? 1.F : 0.F; }

  auto throbSpeed() -> float {
    return taperedAndScaledRotation(this, ThrobSpeedKnob, ThrobSpeedCvInput, ThrobSpeedAvKnob, speedKnobTaper,
                                    speedRange);
  }

  auto wobbleDepth() -> float;
  auto wobblePhase() -> float;
  auto wobbleRatio() -> float;

  Rotor wobbler{};
  Rotor throbber{};
};
} // namespace dhe
