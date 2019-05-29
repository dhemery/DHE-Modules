#include "modules/Blossom.h"

#include "modules/components/Taper.h"
#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"
#include "util/Sigmoid.h"

#include <array>
#include <cmath>

namespace dhe {

static inline auto rotationToSpin(float rotation) -> float {
  auto const tapered = spinKnobTaper.apply(rotation);
  return spinRange.scale(tapered);
}

static inline auto spinToRotation(float spin) -> float {
  auto const tapered = spinRange.normalize(spin);
  return spinKnobTaper.invert(tapered);
}

class SpinKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return rotationToSpin(getValue()); }

  void setDisplayValue(float spin) override { setValue(spinToRotation(spin)); }
};

inline void configSpinKnob(Blossom *blossom, int knobId) {
  static auto constexpr initialSpinHz(1.F);
  static auto const initialSpinKnobRotation = spinToRotation(initialSpinHz);
  blossom->configParam<SpinKnobParamQuantity>(knobId, 0.F, 1.F, initialSpinKnobRotation, "Spin", " Hz");
}

class BounceKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    auto const freeBounceRatio = bounceRange.scale(rotation);
    auto const blossom = dynamic_cast<Blossom *>(module);
    auto const spin = blossom->bounceIsFree() ? freeBounceRatio : std::round(freeBounceRatio);
    return spin;
  }

  void setDisplayValue(float bounceRatio) override {
    auto const rotation = bounceRange.normalize(bounceRatio);
    setValue(rotation);
  }
};

static inline void configBounceKnob(Blossom *blossom, int knobId) {
  blossom->configParam<BounceKnobParamQuantity>(knobId, 0.F, 1.F, knob::centered, "Bounce ratio", " per spin");
}

Blossom::Blossom() {
  config(ParameterCount, InputCount, OutputCount);

  configSpinKnob(this, SpinKnob);
  attenuverter::config(this, SpinAvKNob, "Spin CV gain");

  toggle::config<2>(this, BounceRatioModeSwitch, "Bounce ratio mode", {"Quantized", "Free"}, 1);

  configBounceKnob(this, BounceRatioKnob);
  attenuverter::config(this, BounceRatioAvKnob, "Bounce ratio CV gain");

  knob::configPercentage(this, BounceDepthKnob, "Bounce depth");
  attenuverter::config(this, BounceDepthAvKnob, "Bounce depth CV gain");

  static auto constexpr phaseDisplayRange = Range{-180.F, 180.F};
  knob::config(this, BouncePhaseOffsetKnob, "Bounce phase offset", "°", phaseDisplayRange);
  attenuverter::config(this, BouncePhaseOffsetAvKnob, "Bounce phase offset CV gain");

  gain::config(this, XGainKnob, "X gain");
  level::configSwitch(this, XRangeSwitch, "X range", 0);

  gain::config(this, YGainKnob, "Y gain");
  level::configSwitch(this, YRangeSwitch, "Y range", 0);
}

void Blossom::process(const ProcessArgs &args) {
  auto spinDelta = -spin() * args.sampleTime;
  auto bounceRatio = bounceIsFree() ? bounce() : std::round(bounce());
  auto bounceDepth = knob::rotationRange.clamp(depth());

  spinner.advance(spinDelta, 0.F);
  bouncer.advance(spinDelta * bounceRatio, phase());

  auto angle = spinner.angle();

  auto radius = (1.F - bounceDepth) + bounceDepth * bouncer.radius();
  auto x = radius * std::cos(angle);
  auto const xVoltage = 5.F * xGain() * (x + xOffset());
  outputs[XOutput].setVoltage(xVoltage);

  auto y = radius * std::sin(angle);
  auto const yVoltage = 5.F * yGain() * (y + yOffset());
  outputs[YOutput].setVoltage(yVoltage);
}
} // namespace dhe
