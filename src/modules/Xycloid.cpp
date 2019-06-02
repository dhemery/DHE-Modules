#include "modules/Xycloid.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/LevelConfig.h"
#include "util/Sigmoid.h"

#include <array>

namespace dhe {
static auto constexpr wobbleDepthRange = Range{0.F, 1.F};
static auto constexpr phaseOffsetRange = Range{-180.F, 180.F};
static auto constexpr maxWobbleRatio = 16.F;
static auto constexpr inwardWobbleRatioRange = Range{0.F, -maxWobbleRatio};
static auto constexpr outwardWobbleRatioRange = Range{0.F, maxWobbleRatio};
static auto constexpr bidirectionalWobbleRatioRange = Range{-maxWobbleRatio, maxWobbleRatio};
static auto constexpr initialSpeedHz(1.F);

auto const Xycloid::wobbleRatioRanges
    = std::array<Range const *, 3>{&inwardWobbleRatioRange, &bidirectionalWobbleRatioRange, &outwardWobbleRatioRange};

static inline auto rotationToSpeed(float rotation) -> float {
  auto const tapered = Xycloid::speedKnobTaper.apply(rotation);
  return Xycloid::speedRange.scale(tapered);
}

static inline auto speedToRotation(float speed) -> float {
  auto const tapered = Xycloid::speedRange.normalize(speed);
  return Xycloid::speedKnobTaper.invert(tapered);
}

class SpeedKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return rotationToSpeed(getValue()); }

  void setDisplayValue(float speed) override { setValue(speedToRotation(speed)); }
};

static void configSpeedKnob(Xycloid *xycloid, int knobId) {
  static auto const initialRotation = speedToRotation(initialSpeedHz);
  xycloid->configParam<SpeedKnobParamQuantity>(knobId, 0.F, 1.F, initialRotation, "Throb speed", " Hz");
}

class WobbleRatioKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    auto const freeRatio = xycloid->wobbleRatioRange()->scale(rotation);
    auto const ratio = xycloid->wobbleRatioIsFree() ? freeRatio : std::round(freeRatio);
    return ratio;
  }

  void setDisplayValue(float bounceRatio) override {
    auto const rotation = xycloid->wobbleRatioRange()->normalize(bounceRatio);
    setValue(rotation);
  }

  void initialize(Xycloid *theXycloid) { xycloid = theXycloid; }

private:
  Xycloid *xycloid;
};

void configWobbleRatioKnob(Xycloid *xycloid, int knobId) {
  xycloid->configParam<WobbleRatioKnobParamQuantity>(knobId, 0.F, 1.F, centeredRotation, "Wobble ratio", " per throb");
  auto const paramQuantity = xycloid->paramQuantities[knobId];
  auto const bounceRatioParamQuantity = dynamic_cast<WobbleRatioKnobParamQuantity *>(paramQuantity);
  bounceRatioParamQuantity->initialize(xycloid);
}

Xycloid::Xycloid() {
  config(ParameterCount, InputCount, OutputCount);

  configSpeedKnob(this, ThrobSpeedKnob);
  configAttenuverter(this, ThrobSpeedAvKnob, "Throb speed CV gain");
  configWobbleRatioKnob(this, WobbleRatioKnob);
  configAttenuverter(this, WobbleRatioAvKnob, "Wobble ratio CV gain");
  configToggle<3>(this, WobbleDirectionSwitch, "Wobble direction", {"In", "-In +Out", "Out"}, 2);
  configToggle<2>(this, WobbleRatioModeSwitch, "Wobble ratio mode", {"Quantized", "Free"}, 1);

  configPercentageKnob(this, WobbleDepthKnob, "Wobble depth", {0.F, 1.F});
  configAttenuverter(this, WobbleDepthAvKnob, "Wobble depth CV gain");
  configKnob(this, WobblePhaseOffsetKnob, "Wobble phase offset", "°", phaseOffsetRange);

  configGain(this, XGainKnob, "X gain");
  configLevelRangeSwitch(this, XRangeSwitch, "X range", 0);

  configGain(this, YGainKnob, "Y gain");
  configLevelRangeSwitch(this, YRangeSwitch, "Y range", 0);
}

void Xycloid::process(const ProcessArgs &args) {
  auto const wobbleRatio = this->wobbleRatio();
  auto const wobblePhaseOffset = wobbleRatio < 0.F ? -wobblePhase() : wobblePhase();

  auto const throbSpeed = -this->throbSpeed() * args.sampleTime;
  auto const wobbleSpeed = -wobbleRatio * throbSpeed;
  auto const wobbleDepth = this->wobbleDepth();
  auto const throbDepth = 1.F - wobbleDepth;

  throbber.advance(throbSpeed);
  wobbler.advance(wobbleSpeed, -wobblePhaseOffset);
  auto const x = throbDepth * throbber.x() + wobbleDepth * wobbler.x();
  auto const y = throbDepth * throbber.y() + wobbleDepth * wobbler.y();

  outputs[XOutput].setVoltage(5.F * xGain() * (x + xOffset()));
  outputs[YOutput].setVoltage(5.F * yGain() * (y + yOffset()));
}

auto Xycloid::wobbleDepth() const -> float {
  auto rotation = dhe::rotation(this, WobbleDepthKnob, WobbleDepthCvInput, WobbleDepthAvKnob);
  return wobbleDepthRange.clamp(rotation);
}

auto Xycloid::wobblePhase() const -> float {
  auto rotation = paramValue(this, WobblePhaseOffsetKnob);
  return rotation - 0.5F;
}

auto Xycloid::wobbleRatio() const -> float {
  auto wobbleRatioAmount = rotation(this, WobbleRatioKnob, WobbleRatioCvInput, WobbleRatioAvKnob);
  auto wobbleRatio = wobbleRatioRange()->scale(wobbleRatioAmount);
  return wobbleRatioIsFree() ? wobbleRatio : std::round(wobbleRatio);
}

} // namespace dhe
