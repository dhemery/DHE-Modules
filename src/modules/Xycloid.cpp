#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-constant-array-index"
#include "modules/Xycloid.h"

#include "util/gain.h"
#include "util/sigmoid.h"
#include "util/signal.h"

#include <array>

namespace dhe {

Xycloid::Xycloid() {
  config(ParameterCount, InputCount, OutputCount);

  configParam(ThrobSpeedKnob, 0.f, 1.f, 0.65f, "Throb speed", "", 0.f, 2.f, -1.f);
  configCvGain(ThrobSpeedAvKnob, "Throb speed");

  configKnob(WobbleRatioKnob, "Wobble ratio");
  configCvGain(WobbleRatioAvKnob, "Wobble ratio");
  configParam(WobbleRatioRangeSwitch, 0.f, 2.f, 2.f, "Wobble direction");

  configKnob(WobbleDepthKnob, "Wobble depth");
  configCvGain(WobbleDepthAvKnob, "Wobble depth");

  configParam(WobbleFreedomSwitch, 0.f, 1.f, 1.f, "Wobble freedom");
  configParam(WobblePhaseKnob, 0.f, 1.f, 0.5f, "Wobble phase offset", "°", 0.f, 360.f, -180.f);

  configGain(XGainKnob, "X");
  configSignalRange(XRangeSwitch, "X", false);
  configGain(YGainKnob, "Y");
  configSignalRange(YRangeSwitch, "Y", false);
}

void Xycloid::process(const ProcessArgs &args) {
  auto wobbleRatio = this->wobbleRatio();
  auto wobblePhaseOffset = wobblePhase();
  if (wobbleRatio < 0.f) {
    wobblePhaseOffset *= -1.f;
  }

  auto throbSpeed = this->throbSpeed(args.sampleTime);
  auto wobbleSpeed = wobbleRatio * throbSpeed;
  auto wobbleDepth = this->wobbleDepth();
  auto throbDepth = 1.f - wobbleDepth;

  throbber.advance(throbSpeed);
  wobbler.advance(wobbleSpeed, wobblePhaseOffset);
  auto x = throbDepth * throbber.x() + wobbleDepth * wobbler.x();
  auto y = throbDepth * throbber.y() + wobbleDepth * wobbler.y();

  outputs[XOutput].setVoltage(5.f * xGain() * (x + xOffset()));
  outputs[YOutput].setVoltage(5.f * yGain() * (y + yOffset()));
}

auto Xycloid::wobbleRatioIsFree() -> bool { return params[WobbleFreedomSwitch].getValue() > 0.1f; }

auto Xycloid::offset(int param) -> float {
  auto isUni = params[param].getValue() > 0.5f;
  return isUni ? 1.f : 0.f;
}

auto Xycloid::throbSpeed(float sampleTime) -> float {
  constexpr auto speedTaperCurvature = 0.8f;
  auto rotation = modulated(ThrobSpeedKnob, ThrobSpeedCvInput, ThrobSpeedAvKnob);
  auto scaled = throb_speed_knob_range.scale(rotation);
  auto tapered = sigmoid::inverse(scaled, speedTaperCurvature);
  return -10.f * tapered * sampleTime;
}

auto Xycloid::wobbleDepth() -> float {
  auto rotation = modulated(WobbleDepthKnob, WobbleDepthCvInput, WobbleDepthAvKnob);
  return wobble_depth_range.clamp(rotation);
}

auto Xycloid::wobblePhase() -> float {
  auto rotation = params[WobblePhaseKnob].getValue();
  return rotation - 0.5f;
}

auto Xycloid::wobbleRatioRange() -> const Range & {
  static constexpr auto wobbleRatioMax = 16.f;
  static constexpr auto inwardWobbleRatioRange = Range{0.f, wobbleRatioMax};
  static constexpr auto outwardWobbleRatioRange = Range{0.f, -wobbleRatioMax};
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

#pragma clang diagnostic pop
