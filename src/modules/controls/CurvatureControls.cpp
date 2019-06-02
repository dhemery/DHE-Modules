#include "modules/controls/CurvatureControls.h"

#include "modules/controls/Controls.h"
#include "util/Sigmoid.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {
class KnobParamQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override { return curvature(getValue()); }

  void setDisplayValue(float curvature) override {
    auto const sigmoidClampedCurvature = sigmoid::range.clamp(curvature);
    // Unexpected, but true: Negating the taper curvature inverts the taper.
    auto const sigmoidScaledRotation = sigmoid::curve(sigmoidClampedCurvature, -curvatureKnobTaperCurvature);
    auto const rotation = sigmoid::range.normalize(sigmoidScaledRotation);
    setValue(rotation);
  }
};

void configCurvatureKnob(rack::engine::Module const *module, int knobId, std::string const &name,
                         float initialRotation) {
  nonConst(module)->configParam<KnobParamQuantity>(knobId, 0.F, 1.F, initialRotation, name);
}

void configShapeSwitch(rack::engine::Module const *module, int switchId, std::string const &name, int initialState) {
  static auto const stateNames = std::array<std::string, 2>{"J", "S"};
  configToggle<2>(module, switchId, name, stateNames, initialState);
}

} // namespace dhe
