#pragma once

#include "components/range.h"
#include "components/taper.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace blossom {
static auto constexpr speed_knob_taper_curvature = -0.8F;
static auto constexpr speed_knob_taper =
    taper::FixedSTaper{speed_knob_taper_curvature};
static auto constexpr speed_range = Range{-10.F, 10.F};

static inline auto rotation_to_spin(float rotation) -> float {
  auto const tapered = speed_knob_taper.apply(rotation);
  return speed_range.scale(tapered);
}

static inline auto spin_to_rotation(float spin) -> float {
  auto const tapered = speed_range.normalize(spin);
  return speed_knob_taper.invert(tapered);
}

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
  static auto constexpr initial_spin_hz{1.F};
  static auto const initial_rotation = spin_to_rotation(initial_spin_hz);
  module->configParam<SpeedKnobParamQuantity>(knob_id, 0.F, 1.F,
                                              initial_rotation, "Speed", " Hz");
}

} // namespace blossom
} // namespace dhe
