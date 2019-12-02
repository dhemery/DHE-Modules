#pragma once

namespace dhe {
namespace xycloid {
  static auto constexpr speedKnobTaperCurvature = -0.8F;
  static auto constexpr speedRange = Range{-10.F, 10.F};
  static auto constexpr speedKnobTaper = taper::FixedSTaper{speedKnobTaperCurvature};
  static auto constexpr initialSpeedHz(1.F);

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

  static void configSpeedKnob(rack::engine::Module *module, int knobId) {
    static auto const initialRotation = speedToRotation(initialSpeedHz);
    module->configParam<SpeedKnobParamQuantity>(knobId, 0.F, 1.F, initialRotation, "Speed", " Hz");
  }

} // namespace xycloid
} // namespace dhe