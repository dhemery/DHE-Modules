#pragma once

#include "components/range.h"

#include <rack.hpp>

namespace dhe {
namespace blossom {
static auto constexpr speed_knob_taper_curvature = -0.8F;
static auto constexpr speed_knob_taper =
    sigmoid::s_taper_with_curvature(speed_knob_taper_curvature);
static auto constexpr speed_range = Range{-10.F, 10.F};

static auto constexpr rotation_to_spin(float rotation) -> float {
  return speed_range.scale(speed_knob_taper.apply(rotation));
}

static auto constexpr spin_to_rotation(float spin) -> float {
  return speed_knob_taper.invert(speed_range.normalize(spin));
}

static auto constexpr initial_spin_hz{1.F};
static auto constexpr initial_rotation = spin_to_rotation(initial_spin_hz);

class SpeedKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override {
    return rotation_to_spin(getValue());
  }

  void setDisplayValue(float spin) override {
    setValue(spin_to_rotation(spin));
  }
};

static inline void config_speed_knob(rack::engine::Module *module,
                                     int knob_id) {
  module->configParam<SpeedKnobParamQuantity>(knob_id, 0.F, 1.F,
                                              initial_rotation, "Speed", " Hz");
}

} // namespace blossom
} // namespace dhe
