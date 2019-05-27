#include "modules/Xycloid.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"
#include "util/Sigmoid.h"

#include <array>

namespace dhe {
static auto constexpr wobbleDepthRange = Range{0.F, 1.F};
static auto constexpr phaseOffsetRange = Range{-180.F, 180.F};

namespace speed {
  static auto constexpr knobTaperCurvature = -0.8F;
  static auto constexpr knobTaper = taper::FixedSTaper{knobTaperCurvature};
  static auto constexpr range = Range{-10.F, 10.F};

  inline auto fromRotation(float rotation) -> float {
    auto const tapered = knobTaper.apply(rotation);
    return range.scale(tapered);
  }

  inline auto toRotation(float spin) -> float {
    auto const tapered = range.normalize(spin);
    return knobTaper.invert(tapered);
  }

  class KnobParamQuantity : public rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override { return fromRotation(getValue()); }

    void setDisplayValue(float spin) override { setValue(toRotation(spin)); }
  };

  void config(Xycloid *xycloid, int knobId) {
    static auto constexpr initialSpeedHz(0.5F);
    static auto const initialRotation = toRotation(initialSpeedHz);
    xycloid->configParam<KnobParamQuantity>(knobId, 0.F, 1.F, initialRotation, "Throb speed", " Hz");
  }
} // namespace speed

namespace ratio {
  static constexpr auto maxRatio = 16.F;
  static constexpr auto inward = Range{0.F, -maxRatio};
  static constexpr auto outward = Range{0.F, maxRatio};
  static constexpr auto bidirectional = Range{-maxRatio, maxRatio};
  static constexpr std::array<Range const *, 3> const ranges{&inward, &bidirectional, &outward};

  class KnobParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValue() -> float override {
      auto const rotation = getValue();
      auto const freeRatio = range()->scale(rotation);
      auto const ratio = ratioIsFree() ? freeRatio : std::round(freeRatio);
      return ratio;
    }

    void setDisplayValue(float bounceRatio) override {
      auto const rotation = range()->normalize(bounceRatio);
      setValue(rotation);
    }

    void initialize(std::function<Range const *()> const &wobbleRatioRange,
                    std::function<bool()> const &wobbleRatioIsFree) {
      range = wobbleRatioRange;
      ratioIsFree = wobbleRatioIsFree;
    }

  private:
    std::function<bool()> ratioIsFree;
    std::function<Range const *()> range;
  };

  void config(Xycloid *xycloid, int knobId, std::function<Range const *()> const &wobbleRatioRange,
              std::function<bool()> const &wobbleRatioIsFree) {
    xycloid->configParam<KnobParamQuantity>(knobId, 0.F, 1.F, knob::centered, "Wobble ratio", " per throb");
    auto const paramQuantity = xycloid->paramQuantities[knobId];
    auto const bounceRatioParamQuantity = dynamic_cast<KnobParamQuantity *>(paramQuantity);
    bounceRatioParamQuantity->initialize(wobbleRatioRange, wobbleRatioIsFree);
  }
} // namespace ratio

Xycloid::Xycloid() {
  config(ParameterCount, InputCount, OutputCount);

  speed::config(this, ThrobSpeedKnob);
  attenuverter::config(this, ThrobSpeedAvKnob, "Throb speed CV gain");
  throbSpeed = knob::taperedAndScaled(this, ThrobSpeedKnob, ThrobSpeedCvInput, ThrobSpeedAvKnob, speed::knobTaper,
                                      speed::range);

  wobbleRatioRange = range::selector<3>(this, WobbleDirectionSwitch, ratio::ranges);
  wobbleRatioIsFree = [this]() -> bool { return params[WobbleRatioModeSwitch].getValue() > 0.5F; };
  ratio::config(this, WobbleRatioKnob, wobbleRatioRange, wobbleRatioIsFree);
  attenuverter::config(this, WobbleRatioAvKnob, "Wobble ratio CV gain");
  toggle::config<3>(this, WobbleDirectionSwitch, "Wobble direction", {"In", "-In +Out", "Out"}, 2);
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
  auto const wobbleSpeed = -wobbleRatio * throbSpeed;
  auto const wobbleDepth = this->wobbleDepth();
  auto const throbDepth = 1.F - wobbleDepth;

  throbber.advance(throbSpeed);
  wobbler.advance(wobbleSpeed, wobblePhaseOffset);
  auto const x = throbDepth * throbber.x() + wobbleDepth * wobbler.x();
  auto const y = throbDepth * throbber.y() + wobbleDepth * wobbler.y();

  outputs[XOutput].setVoltage(5.F * xGain() * (x + xOffset()));
  outputs[YOutput].setVoltage(5.F * yGain() * (y + yOffset()));
}

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

auto Xycloid::wobbleRatio() -> float {
  auto wobbleRatioAmount = modulated(WobbleRatioKnob, WobbleRatioCvInput, WobbleRatioAvKnob);
  auto wobbleRatio = wobbleRatioRange()->scale(wobbleRatioAmount);
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
