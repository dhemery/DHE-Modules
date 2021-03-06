#pragma once

#include "common-config.h"
#include "components/sigmoid.h"
#include "config/common-config.h"
#include "controls/curvature-inputs.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>
#include <string>

namespace dhe {
class CurvatureKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return curvature(getValue()); }

  void setDisplayValue(float curvature) override {
    auto const sigmoid_clamped_curvature =
        sigmoid::safe_curvature_range.clamp(curvature);
    // Unexpected, but true: Negating the taper curvature inverts the taper.
    auto const sigmoid_scaled_rotation = sigmoid::curve(
        sigmoid_clamped_curvature, -curvature_knob_taper_curvature);
    auto const rotation = sigmoid::domain.normalize(sigmoid_scaled_rotation);
    setValue(rotation);
  }
};

/**
 * Configures the param and display for a curvature knob.
 */
static inline void
config_curvature_knob(rack::engine::Module *module, int knob_id,
                      std::string const &name = "Curvature",
                      float initial_rotation = centered_rotation) {
  module->configParam<CurvatureKnobParamQuantity>(knob_id, 0.F, 1.F,
                                                  initial_rotation, name);
}

static auto constexpr curvature_state_names =
    std::array<char const *, 2>{"J", "S"};

/**
 * Configures the param and display for a curve shape switch.
 */
static inline void config_curve_shape_switch(rack::engine::Module *module,
                                             int switch_id,
                                             std::string const &name = "Shape",
                                             int initial_state = 0) {
  config_toggle<2>(module, switch_id, name, curvature_state_names,
                   initial_state);
}

} // namespace dhe
