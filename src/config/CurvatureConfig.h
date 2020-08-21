#pragma once

#include "CommonConfig.h"
#include "components/Sigmoid.h"
#include "config/CommonConfig.h"
#include "controls/CurvatureInputs.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>
#include <string>

namespace dhe {
class CurvatureKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return curvature(getValue()); }

  void setDisplayValue(float curvature) override {
    auto const sigmoidClampedCurvature =
        sigmoid::safe_curvature_range.clamp(curvature);
    // Unexpected, but true: Negating the taper curvature inverts the taper.
    auto const sigmoidScaledRotation =
        sigmoid::curve(sigmoidClampedCurvature, -curvatureKnobTaperCurvature);
    auto const rotation = sigmoid::range.normalize(sigmoidScaledRotation);
    setValue(rotation);
  }
};

/**
 * Configures the param and display for a curvature knob.
 */
static inline void
configCurvatureKnob(rack::engine::Module *module, int knobId,
                    std::string const &name = "Curvature",
                    float initialRotation = centered_rotation) {
  module->configParam<CurvatureKnobParamQuantity>(knobId, 0.F, 1.F,
                                                  initialRotation, name);
}

/**
 * Configures the param and display for a curve shape switch.
 */
static inline void configCurveShapeSwitch(rack::engine::Module *module,
                                          int switchId,
                                          std::string const &name = "Shape",
                                          int initialState = 0) {
  static auto const stateNames = std::array<std::string, 2>{"J", "S"};
  config_toggle<2>(module, switchId, name, stateNames, initialState);
}

} // namespace dhe
