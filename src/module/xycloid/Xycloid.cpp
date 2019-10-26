#include "module/xycloid/Xycloid.h"

#include "components/Sigmoid.h"
#include "config/CommonConfig.h"
#include "config/LevelConfig.h"

#include <array>

namespace dhe {
static auto constexpr wobbleDepthRange = Range{0.F, 1.F};
static auto constexpr phaseOffsetRange = Range{-180.F, 180.F};
static auto constexpr maxWobbleRatio = 16.F;
static auto constexpr inwardWobbleRatioRange = Range{0.F, -maxWobbleRatio};
static auto constexpr outwardWobbleRatioRange = Range{0.F, maxWobbleRatio};
static auto constexpr bidirectionalWobbleRatioRange = Range{-maxWobbleRatio, maxWobbleRatio};

static auto constexpr speedKnobTaperCurvature = -0.8F;
static auto constexpr speedRange = Range{-10.F, 10.F};
static auto constexpr speedKnobTaper = taper::FixedSTaper{speedKnobTaperCurvature};
static auto constexpr initialSpeedHz(1.F);

static auto const wobbleRatioRanges
    = std::array<Range const *, 3>{&inwardWobbleRatioRange, &bidirectionalWobbleRatioRange, &outwardWobbleRatioRange};

static inline auto rotationToSpeed(float rotation) -> float {
  auto const tapered = speedKnobTaper.apply(rotation);
  return speedRange.scale(tapered);
}

static inline auto speedToRotation(float speed) -> float {
  auto const tapered = speedRange.normalize(speed);
  return speedKnobTaper.invert(tapered);
}

class SpeedKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return rotationToSpeed(getValue()); }

  void setDisplayValue(float speed) override { setValue(speedToRotation(speed)); }
};

static void configSpeedKnob(Xycloid *xycloid, int knobId) {
  static auto const initialRotation = speedToRotation(initialSpeedHz);
  xycloid->configParam<SpeedKnobParamQuantity>(knobId, 0.F, 1.F, initialRotation, "Speed", " Hz");
}

static inline auto wobbleRatioRange(Xycloid const *xycloid) -> Range const * {
  return selectedRange<3>(xycloid->params[Xycloid::DirectionSwitch], wobbleRatioRanges);
}

static inline auto wobbleRatioIsFree(Xycloid const *xycloid) -> bool {
  return switchPosition(xycloid->params[Xycloid::FreeRatioSwitch]) == 1;
}

static inline auto wobbleRatio(Xycloid const *xycloid, float rotation) -> float {
  auto const wobbleRatio = wobbleRatioRange(xycloid)->scale(rotation);
  return wobbleRatioIsFree(xycloid) ? wobbleRatio : std::round(wobbleRatio);
}

class WobbleRatioKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override { return wobbleRatio(xycloid(), getValue()); }

  void setDisplayValue(float bounceRatio) override {
    auto const range = wobbleRatioRange(xycloid());
    auto const rotation = range->normalize(bounceRatio);
    setValue(rotation);
  }

  auto xycloid() const -> Xycloid const * { return (Xycloid *) module; }
};

void configWobbleRatioKnob(Xycloid *xycloid, int knobId) {
  xycloid->configParam<WobbleRatioKnobParamQuantity>(knobId, 0.F, 1.F, centeredRotation, "Ratio", "x");
}

Xycloid::Xycloid() {
  config(ParameterCount, InputCount, OutputCount);

  configSpeedKnob(this, SpeedKnob);
  configAttenuverter(this, SpeedAvKnob, "Speed CV gain");

  configWobbleRatioKnob(this, RatioKnob);
  configAttenuverter(this, RatioAvKnob, "Ratio CV gain");
  configToggle<3>(this, DirectionSwitch, "Direction", {"In", "-In +Out", "Out"}, 2);
  configToggle<2>(this, FreeRatioSwitch, "Ratio mode", {"Quantized", "Free"}, 1);

  configPercentageKnob(this, DepthKnob, "Depth", {0.F, 1.F});
  configAttenuverter(this, DepthAvKnob, "Depth CV gain");

  configKnob(this, PhaseOffsetKnob, "Phase", "°", phaseOffsetRange);
  configAttenuverter(this, PhaseOffsetAvKnob, "Phase CV gain");

  configGain(this, XGainKnob, "X gain");
  configLevelRangeSwitch(this, XRangeSwitch, "X range", 0);

  configGain(this, YGainKnob, "Y gain");
  configLevelRangeSwitch(this, YRangeSwitch, "Y range", 0);
}

void Xycloid::process(const ProcessArgs &args) {
  auto const wobbleRatio = ratio();
  auto const wobblePhaseOffset = wobbleRatio < 0.F ? -phase() : phase();

  auto const throbSpeed = -speed() * args.sampleTime;
  auto const wobbleSpeed = -wobbleRatio * throbSpeed;
  auto const wobbleDepth = depth();
  auto const throbDepth = 1.F - wobbleDepth;

  throbber.advance(throbSpeed);
  wobbler.advance(wobbleSpeed);
  auto const x = throbDepth * throbber.cos() + wobbleDepth * wobbler.cos(-wobblePhaseOffset);
  auto const y = throbDepth * throbber.sin() + wobbleDepth * wobbler.sin(-wobblePhaseOffset);

  outputs[XOutput].setVoltage(5.F * xGain() * (x + xOffset()));
  outputs[YOutput].setVoltage(5.F * yGain() * (y + yOffset()));
}

auto Xycloid::depth() const -> float {
  return wobbleDepthRange.clamp(rotation(params[DepthKnob], inputs[DepthCvInput], params[DepthAvKnob]));
}

auto Xycloid::phase() const -> float {
  return rotation(params[PhaseOffsetKnob], inputs[PhaseCvInput], params[PhaseOffsetAvKnob]) - 0.5F;
}

auto Xycloid::ratio() const -> float {
  return wobbleRatio(this, rotation(params[RatioKnob], inputs[RatioCvInput], params[RatioAvKnob]));
}

auto Xycloid::speed() const -> float {
  return taperedAndScaledRotation(params[SpeedKnob], inputs[SpeedCvInput], params[SpeedAvKnob], speedKnobTaper,
                                  speedRange);
}

} // namespace dhe
