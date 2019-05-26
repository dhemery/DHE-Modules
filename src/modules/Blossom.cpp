#include "modules/Blossom.h"

#include "modules/components/Taper.h"
#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"
#include "util/Sigmoid.h"

#include <array>

namespace dhe {
static auto constexpr speedCurvature = 0.8F;
static auto constexpr phaseOffsetRange = Range{-180.F, 180.F};
static auto constexpr bounceRatioRange = Range{1.F, 17.F};
static auto constexpr spinRange = Range{-10.F, 10.F};


Blossom::Blossom() {
  config(ParameterCount, InputCount, OutputCount);

  knob::config(this, SpinKnob, "Spin", " Hz", spinRange, 0.65F);
  attenuverter::config(this, SpinAvKNob, "Spin CV gain");

  knob::config(this, BounceRatioKnob, "Bounce ratio", "/spin", bounceRatioRange);
  attenuverter::config(this, BounceRatioAvKnob, "Bounce ratio CV gain");
  toggle::config<2>(this, BounceRatioModeSwitch, "Bounce ratio mode", {"Quantized", "Free"}, 1);

  knob::configPercentage(this, BounceDepthKnob, "Bounce depth", {0.F, 1.F});
  attenuverter::config(this, BounceDepthAvKnob, "Bounce depth CV gain");

  knob::config(this, BouncePhaseOffsetKnob, "Bounce phase offset", "°", phaseOffsetRange);
  attenuverter::config(this, BouncePhaseOffsetAvKnob, "Bounce phase offset CV gain");

  gain::config(this, XGainKnob, "X gain");
  level::configSwitch(this, XRangeSwitch, "X range", 0);

  gain::config(this, YGainKnob, "Y gain");
  level::configSwitch(this, YRangeSwitch, "Y range", 0);
}

void Blossom::process(const ProcessArgs &args) {
  auto spinRate = spin(args.sampleTime);
  auto bounceRatio = isBounceFree() ? bounce() : std::round(bounce());
  auto bounceDepth = depth();

  spinner.advance(spinRate, 0.F);
  bouncer.advance(spinRate * bounceRatio, phase());

  auto angle = spinner.angle();
  auto radius = (1.F - bounceDepth) + bounceDepth * bouncer.radius();

  auto x = radius * std::cos(angle);
  auto y = radius * std::sin(angle);

  outputs[XOutput].setVoltage(5.F * xGain() * (x + xOffset()));
  outputs[YOutput].setVoltage(5.F * yGain() * (y + yOffset()));
}

auto Blossom::offset(int param) -> float {
  auto isUni = params[param].getValue() > 0.5F;
  return isUni ? 1.F : 0.F;
}

auto Blossom::bounce() -> float {
  auto rotation = modulated(BounceRatioKnob, BounceRatioCvInput, BounceRatioAvKnob);
  return bounceRatioRange.scale(rotation);
}

auto Blossom::spin(float sampleTime) -> float {
  static const auto taper = taper::VariableSTaper{};
  auto rotation = modulated(SpinKnob, SpinCvInput, SpinAvKNob);
  auto tapered = taper.apply(rotation, -speedCurvature);
  return spinRange.scale(tapered) * sampleTime;
}

auto Blossom::depth() -> float {
  static constexpr auto depthRange = Range{0.F, 1.F};
  auto rotation = modulated(BounceDepthKnob, BounceDepthCvInput, BounceDepthAvKnob);
  return depthRange.clamp(rotation);
}

auto Blossom::isBounceFree() -> bool { return params[BounceRatioModeSwitch].getValue() > 0.1F; }

auto Blossom::phase() -> float {
  static constexpr auto phaseRange = Range{0.F, 1.F};
  auto rotation = modulated(BouncePhaseOffsetKnob, PhaseCvInput, BouncePhaseOffsetAvKnob);
  return phaseRange.clamp(rotation);
}

auto Blossom::xOffset() -> float { return offset(XRangeSwitch); }

auto Blossom::xGain() -> float {
  float gainAmount = modulated(XGainKnob, XGainCvInput);
  return gain::range.scale(gainAmount);
}

auto Blossom::yGain() -> float {
  float gainAmount = modulated(YGainKnob, YGainCvInput);
  return gain::range.scale(gainAmount);
}

auto Blossom::yOffset() -> float { return offset(YRangeSwitch); }

} // namespace dhe
