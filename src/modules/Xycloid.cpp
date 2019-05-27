#include "modules/Xycloid.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"
#include "util/Sigmoid.h"

#include <array>

namespace dhe {
static auto constexpr throbSpeedKnobCurvature = -0.8F;
static auto constexpr throbSpeedRange = Range{-10.F, 10.F};
static auto constexpr throbSpeedKnobTaper = taper::FixedSTaper{throbSpeedKnobCurvature};
static auto constexpr wobbleDepthRange = Range{0.F, 1.F};
static auto constexpr phaseOffsetRange = Range{-180.F, 180.F};

inline auto rotationToThrobSpeed(float rotation) -> float {
  auto const tapered = throbSpeedKnobTaper.apply(rotation);
  return throbSpeedRange.scale(tapered);
}

inline auto throbSpeedToRotation(float throbSpeed) -> float {
  auto const tapered = throbSpeedRange.normalize(throbSpeed);
  return throbSpeedKnobTaper.invert(tapered);
}

class ThrobSpeedKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return rotationToThrobSpeed(getValue()); }

  void setDisplayValue(float throbSpeed) override { setValue(throbSpeedToRotation(throbSpeed)); }
}; // namespace curvature

Xycloid::Xycloid() {
  config(ParameterCount, InputCount, OutputCount);

  static auto constexpr initialThrobSpeedHz(0.5F);
  auto const initialSpinKnobRotation = throbSpeedToRotation(initialThrobSpeedHz);
  configParam<ThrobSpeedKnobParamQuantity>(ThrobSpeedKnob, 0.F, 1.F, initialSpinKnobRotation, "Throb speed", " Hz");
  attenuverter::config(this, ThrobSpeedAvKnob, "Throb speed CV gain");
  throbSpeed = knob::taperedAndScaled(this, ThrobSpeedKnob, ThrobSpeedCvInput, ThrobSpeedAvKnob, throbSpeedKnobTaper,
                                      throbSpeedRange);

  configParam(WobbleRatioKnob, 0.F, 1.F, 0.5F, "Wobble ratio");
  attenuverter::config(this, WobbleRatioAvKnob, "Wobble ratio CV gain");
  toggle::config<3>(this, WobbleDirectionSwitch, "Wobble direction", {"Inward", "Both", "Outward"}, 2);
  toggle::config<2>(this, WobbleRatioModeSwitch, "Wobble ratio mode", {"Quantized", "Free"}, 1);

  knob::configPercentage(this, WobbleDepthKnob, "Wobble depth", {0.F, 1.F});
  attenuverter::config(this, WobbleDepthAvKnob, "Wobble depth CV gain");
  knob::config(this, WobblePhaseOffsetKnob, "Wobble phase offset", "°", phaseOffsetRange);

  gain::config(this, XGainKnob, "X gain");
  level::configSwitch(this, XRangeSwitch, "X range", 0);

  gain::config(this, YGainKnob, "Y gain");
  level::configSwitch(this, YRangeSwitch, "Y range", 0);
}

void Xycloid::process(const ProcessArgs &args) {
  auto const wobbleRatio = this->wobbleRatio();
  auto const wobblePhaseOffset = wobbleRatio < 0.F ? -wobblePhase() : wobblePhase();

  auto const throbSpeed = -this->throbSpeed() * args.sampleTime;
  auto const wobbleSpeed = wobbleRatio * throbSpeed;
  auto const wobbleDepth = this->wobbleDepth();
  auto const throbDepth = 1.F - wobbleDepth;

  throbber.advance(throbSpeed);
  wobbler.advance(wobbleSpeed, wobblePhaseOffset);
  auto const x = throbDepth * throbber.x() + wobbleDepth * wobbler.x();
  auto const y = throbDepth * throbber.y() + wobbleDepth * wobbler.y();

  outputs[XOutput].setVoltage(5.F * xGain() * (x + xOffset()));
  outputs[YOutput].setVoltage(5.F * yGain() * (y + yOffset()));
}

auto Xycloid::wobbleRatioIsFree() -> bool { return params[WobbleRatioModeSwitch].getValue() > 0.1F; }

auto Xycloid::offset(int param) -> float {
  auto isUni = params[param].getValue() > 0.5F;
  return isUni ? 1.F : 0.F;
}

auto Xycloid::wobbleDepth() -> float {
  auto rotation = modulated(WobbleDepthKnob, WobbleDepthCvInput, WobbleDepthAvKnob);
  return wobbleDepthRange.clamp(rotation);
}

auto Xycloid::wobblePhase() -> float {
  auto rotation = params[WobblePhaseOffsetKnob].getValue();
  return rotation - 0.5F;
}

auto Xycloid::wobbleRatioRange() -> const Range & {
  static constexpr auto wobbleRatioMax = 16.F;
  static constexpr auto inwardWobbleRatioRange = Range{0.F, wobbleRatioMax};
  static constexpr auto outwardWobbleRatioRange = Range{0.F, -wobbleRatioMax};
  static constexpr auto bidirectionalWobbleRatioRange = Range{wobbleRatioMax, -wobbleRatioMax};
  static constexpr std::array<Range, 3> wobbleRatioRanges{inwardWobbleRatioRange, bidirectionalWobbleRatioRange,
                                                          outwardWobbleRatioRange};

  const auto param = params[WobbleDirectionSwitch].getValue();
  const auto selection = static_cast<int>(param);

  return wobbleRatioRanges[selection];
}

auto Xycloid::wobbleRatio() -> float {
  auto wobbleRatioAmount = modulated(WobbleRatioKnob, WobbleRatioCvInput, WobbleRatioAvKnob);
  auto wobbleRatio = wobbleRatioRange().scale(wobbleRatioAmount);
  return wobbleRatioIsFree() ? wobbleRatio : std::round(wobbleRatio);
}

auto Xycloid::xGain() -> float {
  float gainAmount = modulated(XGainKnob, XGainCvInput);
  return gain::range.scale(gainAmount);
}

auto Xycloid::xOffset() -> float { return offset(XRangeSwitch); }

auto Xycloid::yGain() -> float {
  float gainAmount = modulated(YGainKnob, YGainCvInput);
  return gain::range.scale(gainAmount);
}

auto Xycloid::yOffset() -> float { return offset(YRangeSwitch); }

} // namespace dhe
