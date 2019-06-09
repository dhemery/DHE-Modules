#pragma once

#include "modules/components/Rotor.h"
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
  inline auto ratio() const -> float { return scaledRotation(this, RatioKnob, RatioCvInput, RatioAvKnob, ratioRange); }

  inline auto ratioIsFree() const -> bool { return switchPosition(this, FreeRatioSwitch) == 1; }

  inline auto depth() const -> float { return rotation(this, DepthKnob, DepthCvInput, DepthAvKnob); }

  inline auto phase() const -> float {
    return scaledRotation(this, PhaseOffsetKnob, PhaseCvInput, PhaseOffsetAvKnob, {-0.5, 0.5});
  }

  inline auto speed() const -> float {
    return taperedAndScaledRotation(this, SpeedKnob, SpeedCvInput, SpeedAvKNob, speedKnobTaper, speedRange);
  }

  inline auto xGain() const -> float { return scaledRotation(this, XGainKnob, XGainCvInput, gainRange); }
  inline auto xOffset() const -> float { return selected<float, 2>(this, XRangeSwitch, {0.F, 1.F}); };

  inline auto yGain() const -> float { return scaledRotation(this, YGainKnob, YGainCvInput, gainRange); }
  inline auto yOffset() const -> float { return selected<float, 2>(this, YRangeSwitch, {0.F, 1.F}); };

  Rotor spinner{};
  Rotor bouncer{};
};
} // namespace dhe
