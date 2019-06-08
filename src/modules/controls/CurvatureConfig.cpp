#include "modules/controls/CommonConfig.h"
#include "modules/controls/CurvatureInputs.h"
#include "util/Sigmoid.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {
class CurvatureKnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return curvature(getValue()); }

  void setDisplayValue(float curvature) override {
    auto const sigmoidClampedCurvature = sigmoid::safeCurvatureRange.clamp(curvature);
    // Unexpected, but true: Negating the taper curvature inverts the taper.
    auto const sigmoidScaledRotation = sigmoid::curve(sigmoidClampedCurvature, -curvatureKnobTaperCurvature);
    auto const rotation = sigmoid::range.normalize(sigmoidScaledRotation);
    setValue(rotation);
  }
};

void configCurvatureKnob(rack::engine::Module *module, int knobId, std::string const &name, float initialRotation) {
  module->configParam<CurvatureKnobParamQuantity>(knobId, 0.F, 1.F, initialRotation, name);
}

void configCurveShapeSwitch(rack::engine::Module *module, int switchId, std::string const &name, int initialState) {
  static auto const stateNames = std::array<std::string, 2>{"J", "S"};
  configToggle<2>(module, switchId, name, stateNames, initialState);
}

} // namespace dhe
