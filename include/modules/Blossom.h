#pragma once

#include "modules/components/PhaseAccumulator.h"
#include "modules/controls/CommonInputs.h"

#include <engine/Module.hpp>

namespace dhe {

static auto constexpr ratioRange = Range{1.F, 17.F};
static auto constexpr speedKnobTaperCurvature = -0.8F;
static auto constexpr speedKnobTaper = taper::FixedSTaper{speedKnobTaperCurvature};
static auto constexpr speedRange = Range{-10.F, 10.F};

class Blossom : public rack::engine::Module {
public:
  Blossom();
  void process(const ProcessArgs &args) override;

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

private:
  inline auto ratio() const -> float {
    return ratioRange.scale(rotation(params[RatioKnob], inputs[RatioCvInput], params[RatioAvKnob]));
  }

  inline auto ratioIsFree() const -> bool { return switchPosition(params[FreeRatioSwitch]) == 1; }

  inline auto depth() const -> float { return rotation(params[DepthKnob], inputs[DepthCvInput], params[DepthAvKnob]); }

  inline auto phaseOffset() const -> float {
    static auto constexpr phaseRange = Range{-0.5F, 0.5F};
    return phaseRange.scale(rotation(params[PhaseOffsetKnob], inputs[PhaseCvInput], params[PhaseOffsetAvKnob]));
  }

  inline auto speed() const -> float {
    return taperedAndScaledRotation(params[SpeedKnob], inputs[SpeedCvInput], params[SpeedAvKNob], speedKnobTaper,
                                    speedRange);
  }

  inline auto xGain() const -> float { return gainRange.scale(rotation(params[XGainKnob], inputs[XGainCvInput])); }
  inline auto xOffset() const -> float { return selected<float, 2>(params[XRangeSwitch], {0.F, 1.F}); };

  inline auto yGain() const -> float { return gainRange.scale(rotation(params[YGainKnob], inputs[YGainCvInput])); }
  inline auto yOffset() const -> float { return selected<float, 2>(params[YRangeSwitch], {0.F, 1.F}); };

  PhaseAccumulator spinner{};
  PhaseAccumulator bouncer{};
};
} // namespace dhe
