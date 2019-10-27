#include "Blossom.h"

#include "components/Sigmoid.h"
#include "config/LevelConfig.h"

#include <cmath>

namespace dhe {

static inline auto rotationToSpin(float rotation) -> float {
  auto const tapered = speedKnobTaper.apply(rotation);
  return speedRange.scale(tapered);
}

static inline auto spinToRotation(float spin) -> float {
  auto const tapered = speedRange.normalize(spin);
  return speedKnobTaper.invert(tapered);
}

class SpinKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return rotationToSpin(getValue()); }

  void setDisplayValue(float spin) override { setValue(spinToRotation(spin)); }
};

static inline void configSpeedKnob(Blossom *blossom, int knobId) {
  static auto constexpr initialSpinHz(1.F);
  static auto const initialSpinKnobRotation = spinToRotation(initialSpinHz);
  blossom->configParam<SpinKnobParamQuantity>(knobId, 0.F, 1.F, initialSpinKnobRotation, "Speed", " Hz");
}

class BounceKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    auto const freeBounceRatio = ratioRange.scale(rotation);
    auto const blossom = dynamic_cast<Blossom *>(module);
    auto const spin
        = positionOf(blossom->params[Blossom::FreeRatioSwitch]) == 1 ? freeBounceRatio : std::round(freeBounceRatio);
    return spin;
  }

  void setDisplayValue(float bounceRatio) override {
    auto const rotation = ratioRange.normalize(bounceRatio);
    setValue(rotation);
  }
};

static inline void configBounceKnob(Blossom *blossom, int knobId) {
  blossom->configParam<BounceKnobParamQuantity>(knobId, 0.F, 1.F, centeredRotation, "Ratio", "x");
}

Blossom::Blossom() {
  config(ParameterCount, InputCount, OutputCount);

  configSpeedKnob(this, SpeedKnob);
  configAttenuverter(this, SpeedAvKNob, "Speed CV gain");

  configToggle<2>(this, FreeRatioSwitch, "Ratio mode", {"Quantized", "Free"}, 1);

  configBounceKnob(this, RatioKnob);
  configAttenuverter(this, RatioAvKnob, "Ratio CV gain");

  configPercentageKnob(this, DepthKnob, "Depth");
  configAttenuverter(this, DepthAvKnob, "Depth CV gain");

  static auto constexpr phaseDisplayRange = Range{-180.F, 180.F};
  configKnob(this, PhaseOffsetKnob, "Phase", "°", phaseDisplayRange);
  configAttenuverter(this, PhaseOffsetAvKnob, "Phase CV gain");

  configGain(this, XGainKnob, "X gain");
  configLevelRangeSwitch(this, XRangeSwitch, "X range", 0);

  configGain(this, YGainKnob, "Y gain");
  configLevelRangeSwitch(this, YRangeSwitch, "Y range", 0);
}

void Blossom::process(const ProcessArgs &args) {
  auto const spinDelta = -speed() * args.sampleTime;
  auto const bounceRatio = ratioIsFree() ? ratio() : std::round(ratio());
  auto const bounceDepth = rotationRange.clamp(depth());

  spinner.advance(spinDelta);
  bouncer.advance(spinDelta * bounceRatio);

  auto const angle = spinner.angle();

  auto const radius = (1.F - bounceDepth) + bounceDepth * bouncer.sin(phaseOffset());
  auto const x = radius * std::cos(angle);
  auto const xVoltage = 5.F * xGain() * (x + xOffset());
  outputs[XOutput].setVoltage(xVoltage);

  auto const y = radius * std::sin(angle);
  auto const yVoltage = 5.F * yGain() * (y + yOffset());
  outputs[YOutput].setVoltage(yVoltage);
}
} // namespace dhe
