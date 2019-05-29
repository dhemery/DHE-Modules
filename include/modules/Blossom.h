#pragma once

#include "Module.h"
#include "modules/components/Rotor.h"
#include "modules/controls/Controls.h"
#include "modules/controls/Inputs.h"

#include <functional>

namespace dhe {

static auto constexpr bounceRange = Range{1.F, 17.F};
static auto constexpr spinKnobTaperCurvature = -0.8F;
static auto constexpr spinKnobTaper = taper::FixedSTaper{spinKnobTaperCurvature};
static auto constexpr spinRange = Range{-10.F, 10.F};

class Blossom : public rack::engine::Module {
public:
  Blossom();
  void process(const ProcessArgs &args) override;
  inline auto bounceIsFree() -> bool { return position(this, BounceRatioModeSwitch) == 1; }

  enum ParameterIds {
    SpinKnob,
    SpinAvKNob,
    BounceRatioKnob,
    BounceRatioAvKnob,
    BounceRatioModeSwitch,
    BounceDepthKnob,
    BounceDepthAvKnob,
    BouncePhaseOffsetKnob,
    BouncePhaseOffsetAvKnob,
    XGainKnob,
    YGainKnob,
    XRangeSwitch,
    YRangeSwitch,
    ParameterCount
  };
  enum InputIds {
    SpinCvInput,
    BounceRatioCvInput,
    BounceDepthCvInput,
    BouncePhaseCvInput,
    XGainCvInput,
    YGainCvInput,
    InputCount
  };
  enum OutputIds { XOutput, YOutput, OutputCount };

private:
  inline auto bounce() -> float {
    return scaled(this, BounceRatioKnob, BounceRatioCvInput, BounceRatioAvKnob, bounceRange);
  }

  inline auto depth() -> float { return rotation(this, BounceDepthKnob, BounceDepthCvInput, BounceDepthAvKnob); }

  inline auto phase() -> float {
    return scaled(this, BouncePhaseOffsetKnob, BouncePhaseCvInput, BouncePhaseOffsetAvKnob, {-0.5, 0.5});
  }

  inline auto spin() -> float {
    return taperedAndScaled(this, SpinKnob, SpinCvInput, SpinAvKNob, spinKnobTaper, spinRange);
  }

  inline auto xGain() -> float { return scaled(this, XGainKnob, XGainCvInput, gain::range); }
  inline auto xOffset() -> float { return selected<float, 2>(this, XRangeSwitch, {0.F, 1.F}); };

  inline auto yGain() -> float { return scaled(this, YGainKnob, YGainCvInput, gain::range); }
  inline auto yOffset() -> float { return selected<float, 2>(this, YRangeSwitch, {0.F, 1.F}); };

  Rotor spinner{};
  Rotor bouncer{};
};
} // namespace dhe
