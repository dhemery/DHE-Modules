#include "modules/Blossom.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"
#include "util/Gain.h"
#include "util/Sigmoid.h"

#include <array>

namespace dhe {
static auto constexpr speedCurvature = 0.8F;

Blossom::Blossom() {
  config(ParameterCount, InputCount, OutputCount);

  configParam(SpinKnob, 0.F, 1.F, 0.65F, "Spin", "", 0.F, 2.F, -1.F);
  attenuverter::config(this, SpinAvKNob, "Spin CV gain");

  configKnob(BounceRatioKnob, "Bounce ratio");
  attenuverter::config(this, BounceRatioAvKnob, "Bounce ratio CV gain");
  toggle::config<2>(this, BounceRatioModeSwitch, "Bounce ratio mode", {"Quantized", "Free"}, 1);

  configParam(BounceDepthKnob, 0.F, 1.F, 0.5F, "Bounce depth", "%", 0.F, 100.F);
  attenuverter::config(this, BounceDepthAvKnob, "Bounce depth CV gain");

  configParam(PhaseKnob, 0.F, 1.F, 0.5F, "Bounce phase offset", "°", 0.F, 360.F, -180.F);
  attenuverter::config(this, PhaseAvKnob, "Bounce phase offset CV gain");

  configGain(XGainKnob, "X");
  level::configSwitch(this, XRangeSwitch, "X range", 0);

  configGain(YGainKnob, "Y");
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
  static constexpr auto bounceRange = Range{1.F, 17.F};
  auto rotation = modulated(BounceRatioKnob, BounceRatioCvInput, BounceRatioAvKnob);
  return bounceRange.scale(rotation);
}

auto Blossom::spin(float sampleTime) -> float {
  static constexpr auto spinRange = Range{-1.F, 1.F};
  auto rotation = modulated(SpinKnob, SpinCvInput, SpinAvKNob);
  auto scaled = spinRange.scale(rotation);
  auto tapered = sigmoid::inverse(scaled, speedCurvature);
  return -10.F * tapered * sampleTime;
}

auto Blossom::depth() -> float {
  static constexpr auto depthRange = Range{0.F, 1.F};
  auto rotation = modulated(BounceDepthKnob, BounceDepthCvInput, BounceDepthAvKnob);
  return depthRange.clamp(rotation);
}

auto Blossom::isBounceFree() -> bool { return params[BounceRatioModeSwitch].getValue() > 0.1F; }

auto Blossom::phase() -> float {
  static constexpr auto phaseRange = Range{0.F, 1.F};
  auto rotation = modulated(PhaseKnob, PhaseCvInput, PhaseAvKnob);
  return phaseRange.clamp(rotation);
}

auto Blossom::xOffset() -> float { return offset(XRangeSwitch); }

auto Blossom::xGain() -> float { return gain::multiplier(modulated(XGainKnob, XGainCvInput)); }

auto Blossom::yGain() -> float { return gain::multiplier(modulated(YGainKnob, YGainCvInput)); }

auto Blossom::yOffset() -> float { return offset(YRangeSwitch); }

} // namespace dhe
