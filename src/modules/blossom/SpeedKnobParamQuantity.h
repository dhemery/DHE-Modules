#pragma once

#include "components/range.h"
#include "components/taper.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace blossom {
static auto constexpr speedKnobTaperCurvature = -0.8F;
static auto constexpr speedKnobTaper =
    taper::FixedSTaper{speedKnobTaperCurvature};
static auto constexpr speedRange = Range{-10.F, 10.F};

static inline auto rotationToSpin(float rotation) -> float {
  auto const tapered = speedKnobTaper.apply(rotation);
  return speedRange.scale(tapered);
}

static inline auto spinToRotation(float spin) -> float {
  auto const tapered = speedRange.normalize(spin);
  return speedKnobTaper.invert(tapered);
}

class SpeedKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override {
    return rotationToSpin(getValue());
  }

  void setDisplayValue(float spin) override { setValue(spinToRotation(spin)); }
};

static inline void configSpeedKnob(rack::engine::Module *module, int knobId) {
  static auto constexpr initialSpinHz(1.F);
  static auto const initialRotation = spinToRotation(initialSpinHz);
  module->configParam<SpeedKnobParamQuantity>(knobId, 0.F, 1.F, initialRotation,
                                              "Speed", " Hz");
}

} // namespace blossom
} // namespace dhe
