#include "modules/Blossom.h"

#include "modules/components/Taper.h"
#include "modules/controls/LevelConfig.h"
#include "util/Sigmoid.h"

#include <array>
#include <cmath>

namespace dhe {

static inline auto rotationToSpeed(float rotation) -> float {
  auto const tapered = spinKnobTaper.apply(rotation);
  return spinRange.scale(tapered);
}

static inline auto speedToRotation(float spin) -> float {
  auto const tapered = spinRange.normalize(spin);
  return spinKnobTaper.invert(tapered);
}

class SpinKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return rotationToSpeed(getValue()); }

  void setDisplayValue(float spin) override { setValue(speedToRotation(spin)); }
};

inline void configSpinKnob(Blossom *blossom, int knobId) {
  static auto constexpr initialSpinHz(1.F);
  static auto const initialSpinKnobRotation = speedToRotation(initialSpinHz);
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
  blossom->configParam<BounceKnobParamQuantity>(knobId, 0.F, 1.F, centeredRotation, "Bounce ratio", " per spin");
}

Blossom::Blossom() {
  config(ParameterCount, InputCount, OutputCount);

  configSpinKnob(this, SpinKnob);
  configAttenuverter(this, SpinAvKNob, "Spin CV gain");

  configToggle<2>(this, BounceRatioModeSwitch, "Bounce ratio mode", {"Quantized", "Free"}, 1);

  configBounceKnob(this, BounceRatioKnob);
  configAttenuverter(this, BounceRatioAvKnob, "Bounce ratio CV gain");

  configPercentageKnob(this, BounceDepthKnob, "Bounce depth");
  configAttenuverter(this, BounceDepthAvKnob, "Bounce depth CV gain");

  static auto constexpr phaseDisplayRange = Range{-180.F, 180.F};
  configKnob(this, BouncePhaseOffsetKnob, "Bounce phase offset", "°", phaseDisplayRange);
  configAttenuverter(this, BouncePhaseOffsetAvKnob, "Bounce phase offset CV gain");

  configGain(this, XGainKnob, "X gain");
  configLevelRangeSwitch(this, XRangeSwitch, "X range", 0);

  configGain(this, YGainKnob, "Y gain");
  configLevelRangeSwitch(this, YRangeSwitch, "Y range", 0);
}

void Blossom::process(const ProcessArgs &args) {
  auto const spinDelta = -spin() * args.sampleTime;
  auto const bounceRatio = bounceIsFree() ? bounce() : std::round(bounce());
  auto const bounceDepth = rotationRange.clamp(depth());

  spinner.advance(spinDelta, 0.F);
  bouncer.advance(spinDelta * bounceRatio, phase());

  auto const angle = spinner.angle();

  auto const radius = (1.F - bounceDepth) + bounceDepth * bouncer.radius();
  auto const x = radius * std::cos(angle);
  auto const xVoltage = 5.F * xGain() * (x + xOffset());
  outputs[XOutput].setVoltage(xVoltage);

  auto const y = radius * std::sin(angle);
  auto const yVoltage = 5.F * yGain() * (y + yOffset());
  outputs[YOutput].setVoltage(yVoltage);
}
} // namespace dhe
