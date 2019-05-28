#pragma once

#include "Module.h"
#include "modules/components/Rotor.h"
#include "modules/controls/Controls.h"

#include <functional>

namespace dhe {

static auto constexpr bounceRange = Range{1.F, 17.F};
static auto constexpr spinKnobTaperCurvature = -0.8F;
static auto constexpr spinKnobTaper = taper::FixedSTaper{spinKnobTaperCurvature};
static auto constexpr spinRange = Range{-10.F, 10.F};

class Blossom : public Module {
public:
  Blossom();
  void process(const ProcessArgs &args) override;

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
  std::function<float()> const bounce{
      knob::scaled(this, BounceRatioKnob, BounceRatioCvInput, BounceRatioAvKnob, bounceRange)};
  std::function<bool()> const bounceIsFree{button::state(this, BounceRatioModeSwitch)};
  std::function<float()> const depth{knob::rotation(this, BounceDepthKnob, BounceDepthCvInput, BounceDepthAvKnob)};
  std::function<float()> const phase{
      knob::scaled(this, BouncePhaseOffsetKnob, BouncePhaseCvInput, BouncePhaseOffsetAvKnob, {-0.5, 0.5})};
  std::function<float()> const spin{
      knob::taperedAndScaled(this, SpinKnob, SpinCvInput, SpinAvKNob, spinKnobTaper, spinRange)};
  std::function<float()> const xGain{knob::scaled(this, XGainKnob, XGainCvInput, gain::range)};
  std::function<float()> const xOffset{selection::of<float, 2>(this, XRangeSwitch, {0.F, 1.F})};
  std::function<float()> const yGain{knob::scaled(this, YGainKnob, YGainCvInput, gain::range)};
  std::function<float()> const yOffset{selection::of<float, 2>(this, YRangeSwitch, {0.F, 1.F})};

  Rotor spinner{};
  Rotor bouncer{};
};
} // namespace dhe
