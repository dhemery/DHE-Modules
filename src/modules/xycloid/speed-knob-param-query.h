#pragma once

#include "components/range.h"
#include "components/sigmoid.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace xycloid {
static auto constexpr speed_knob_taper_curvature = -0.8F;
static auto constexpr speed_knob_taper =
    sigmoid::s_taper_with_curvature(speed_knob_taper_curvature);
static auto constexpr speed_range = Range{-10.F, 10.F};

static inline auto rotation_to_speed(float rotation) -> float {
  auto const tapered = speed_knob_taper.apply(rotation);
  return speed_range.scale(tapered);
}

static inline auto speed_to_rotation(float speed) -> float {
  auto const tapered = speed_range.normalize(speed);
  return speed_knob_taper.invert(tapered);
}

class SpeedKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override {
    return rotation_to_speed(getValue());
  }

  void setDisplayValue(float speed) override {
    setValue(speed_to_rotation(speed));
  }
};

static void config_speed_knob(rack::engine::Module *module, int knob_id) {
  static auto constexpr initial_speed_hz{1.F};
  static auto const initial_rotation = speed_to_rotation(initial_speed_hz);
  module->configParam<SpeedKnobParamQuantity>(knob_id, 0.F, 1.F,
                                              initial_rotation, "Speed", " Hz");
}

} // namespace xycloid
} // namespace dhe
