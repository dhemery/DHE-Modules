#include "modules/Blossom.h"

#include "modules/components/Taper.h"
#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"
#include "util/Sigmoid.h"

#include <array>
#include <cmath>

namespace dhe {
namespace spin {
  static auto constexpr knobTaperCurvature = -0.8F;
  static auto constexpr knobTaper = taper::FixedSTaper{knobTaperCurvature};
  static auto constexpr range = Range{-10.F, 10.F};
  static auto constexpr initialSpinHz(1.F);

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

  void config(Blossom *blossom, int knobId) {
    static auto const initialSpinKnobRotation = spin::toRotation(initialSpinHz);
    blossom->configParam<spin::KnobParamQuantity>(knobId, 0.F, 1.F, initialSpinKnobRotation, "Spin", " Hz");
  }
} // namespace spin

namespace bounce {
  static auto constexpr range = Range{1.F, 17.F};

  class KnobParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValue() -> float override {
      auto const rotation = getValue();
      auto const freeBounceRatio = range.scale(rotation);
      auto const spin = bounceIsFree() ? freeBounceRatio : std::round(freeBounceRatio);
      return spin;
    }

    void setDisplayValue(float bounceRatio) override {
      auto const rotation = range.normalize(bounceRatio);
      setValue(rotation);
    }

    void initialize(std::function<bool()> const &bounceIsFree) { this->bounceIsFree = bounceIsFree; }

  private:
    std::function<bool()> bounceIsFree;
  };

  void config(Blossom *blossom, int knobId, std::function<bool()> const &bounceIsFree) {
    blossom->configParam<bounce::KnobParamQuantity>(knobId, 0.F, 1.F, knob::centered, "Bounce ratio", " per spin");
    auto const paramQuantity = blossom->paramQuantities[knobId];
    auto const bounceRatioParamQuantity = dynamic_cast<bounce::KnobParamQuantity *>(paramQuantity);
    bounceRatioParamQuantity->initialize(bounceIsFree);
  }
} // namespace bounce

namespace phase {
  static auto constexpr displayRange = Range{-180.F, 180.F};
}

Blossom::Blossom() {
  config(ParameterCount, InputCount, OutputCount);

  spin::config(this, SpinKnob);
  attenuverter::config(this, SpinAvKNob, "Spin CV gain");
  spin = knob::taperedAndScaled(this, SpinKnob, SpinCvInput, SpinAvKNob, spin::knobTaper, spin::range);

  toggle::config<2>(this, BounceRatioModeSwitch, "Bounce ratio mode", {"Quantized", "Free"}, 1);
  bounceIsFree = [this]() -> bool { return params[BounceRatioModeSwitch].getValue() > 0.1F; };

  bounce::config(this, BounceRatioKnob, bounceIsFree);
  attenuverter::config(this, BounceRatioAvKnob, "Bounce ratio CV gain");
  bounce = knob::scaled(this, BounceRatioKnob, BounceRatioCvInput, BounceRatioAvKnob, bounce::range);

  knob::configPercentage(this, BounceDepthKnob, "Bounce depth", {0.F, 1.F});
  attenuverter::config(this, BounceDepthAvKnob, "Bounce depth CV gain");
  depth = knob::rotation(this, BounceDepthKnob, BounceDepthCvInput, BounceDepthAvKnob);

  knob::config(this, BouncePhaseOffsetKnob, "Bounce phase offset", "°", phase::displayRange);
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

auto Blossom::phase() -> float {
  auto const rotation = modulated(BouncePhaseOffsetKnob, BouncePhaseCvInput, BouncePhaseOffsetAvKnob);
  return rotation - 0.5F;
}

auto Blossom::xOffset() -> float { return params[XRangeSwitch].getValue(); }

auto Blossom::xGain() -> float {
  float gainAmount = modulated(XGainKnob, XGainCvInput);
  return gain::range.scale(gainAmount);
}

auto Blossom::yGain() -> float {
  float gainAmount = modulated(YGainKnob, YGainCvInput);
  return gain::range.scale(gainAmount);
}

auto Blossom::yOffset() -> float { return params[YRangeSwitch].getValue(); }

} // namespace dhe
