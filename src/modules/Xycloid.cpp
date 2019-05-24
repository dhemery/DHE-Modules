#include "modules/Xycloid.h"

#include "util/gain.h"
#include "util/sigmoid.h"

#include <array>

namespace dhe {

Xycloid::Xycloid() {
  config(ParameterCount, InputCount, OutputCount);

  configParam(ThrobSpeedKnob, 0.F, 1.F, 0.65F, "Throb speed", "", 0.F, 2.F, -1.F);
  configCvGain(ThrobSpeedAvKnob, "Throb speed");

  configKnob(WobbleRatioKnob, "Wobble ratio");
  configCvGain(WobbleRatioAvKnob, "Wobble ratio");
  configParam(WobbleRatioRangeSwitch, 0.F, 2.F, 2.F, "Wobble direction");

  configKnob(WobbleDepthKnob, "Wobble depth");
  configCvGain(WobbleDepthAvKnob, "Wobble depth");

  configParam(WobbleFreedomSwitch, 0.F, 1.F, 1.F, "Wobble freedom");
  configParam(WobblePhaseKnob, 0.F, 1.F, 0.5F, "Wobble phase offset", "°", 0.F, 360.F, -180.F);

  configGain(XGainKnob, "X");
  configSignalRange(XRangeSwitch, "X", false);
  configGain(YGainKnob, "Y");
  configSignalRange(YRangeSwitch, "Y", false);
}

void Xycloid::process(const ProcessArgs &args) {
  auto wobbleRatio = this->wobbleRatio();
  auto wobblePhaseOffset = wobblePhase();
  if (wobbleRatio < 0.F) {
    wobblePhaseOffset *= -1.F;
  }

  auto throbSpeed = this->throbSpeed(args.sampleTime);
  auto wobbleSpeed = wobbleRatio * throbSpeed;
  auto wobbleDepth = this->wobbleDepth();
  auto throbDepth = 1.F - wobbleDepth;

  throbber.advance(throbSpeed);
  wobbler.advance(wobbleSpeed, wobblePhaseOffset);
  auto x = throbDepth * throbber.x() + wobbleDepth * wobbler.x();
  auto y = throbDepth * throbber.y() + wobbleDepth * wobbler.y();

  outputs[XOutput].setVoltage(5.F * xGain() * (x + xOffset()));
  outputs[YOutput].setVoltage(5.F * yGain() * (y + yOffset()));
}

auto Xycloid::wobbleRatioIsFree() -> bool { return params[WobbleFreedomSwitch].getValue() > 0.1F; }

auto Xycloid::offset(int param) -> float {
  auto isUni = params[param].getValue() > 0.5F;
  return isUni ? 1.F : 0.F;
}

auto Xycloid::throbSpeed(float sampleTime) -> float {
  constexpr auto speedTaperCurvature = 0.8F;
  auto rotation = modulated(ThrobSpeedKnob, ThrobSpeedCvInput, ThrobSpeedAvKnob);
  auto scaled = throb_speed_knob_range.scale(rotation);
  auto tapered = sigmoid::inverse(scaled, speedTaperCurvature);
  return -10.F * tapered * sampleTime;
}

auto Xycloid::wobbleDepth() -> float {
  auto rotation = modulated(WobbleDepthKnob, WobbleDepthCvInput, WobbleDepthAvKnob);
  return wobble_depth_range.clamp(rotation);
}

auto Xycloid::wobblePhase() -> float {
  auto rotation = params[WobblePhaseKnob].getValue();
  return rotation - 0.5F;
}

auto Xycloid::wobbleRatioRange() -> const Range & {
  static constexpr auto wobbleRatioMax = 16.F;
  static constexpr auto inwardWobbleRatioRange = Range{0.F, wobbleRatioMax};
  static constexpr auto outwardWobbleRatioRange = Range{0.F, -wobbleRatioMax};
  static constexpr auto bidirectionalWobbleRatioRange = Range{wobbleRatioMax, -wobbleRatioMax};
  static constexpr std::array<Range, 3> wobbleRatioRanges{inwardWobbleRatioRange, bidirectionalWobbleRatioRange,
                                                          outwardWobbleRatioRange};

  const auto param = params[WobbleRatioRangeSwitch].getValue();
  const auto selection = static_cast<int>(param);

  return wobbleRatioRanges[selection];
}

auto Xycloid::wobbleRatio() -> float {
  auto wobbleRatioAmount = modulated(WobbleRatioKnob, WobbleRatioCvInput, WobbleRatioAvKnob);
  auto wobbleRatio = wobbleRatioRange().scale(wobbleRatioAmount) + wobble_ratio_offset;
  return wobbleRatioIsFree() ? wobbleRatio : std::round(wobbleRatio);
}

auto Xycloid::xGain() -> float { return Gain::multiplier(modulated(XGainKnob, XGainCvInput)); }

auto Xycloid::xOffset() -> float { return offset(XRangeSwitch); }

auto Xycloid::yGain() -> float { return Gain::multiplier(modulated(YGainKnob, YGainCvInput)); }

auto Xycloid::yOffset() -> float { return offset(YRangeSwitch); }

} // namespace dhe
