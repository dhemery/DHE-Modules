#include <util/range.h>

#include <engine/ParamQuantity.hpp>

#include "modules/controls/Curvature.h"
#include "modules/params/CurveParams.h"

namespace dhe {
namespace curvature {

class ShapeSwitchParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValueString() -> std::string override {
    return getValue() < 0.5 ? "J" : "S";
  }
};

class KnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    static auto const curvatureTaperFor = curvature::rotationToCurvature();
    return curvatureTaperFor(getValue());
  }

  void setDisplayValue(float taperedCurvature) override {
    static auto const rotationFor = curvature::curvatureToRotation();
    setValue(rotationFor(taperedCurvature));
  }
};

void configKnob(rack::engine::Module *module, int knobId,
                std::string const &name, float initialPosition) {
  module->configParam<KnobParamQuantity>(knobId, 0.f, 1.f, initialPosition,
                                         name);
}

void configSwitch(rack::engine::Module *module, int switchId,
                  std::string const &name, int initialPosition) {
  module->configParam<ShapeSwitchParamQuantity>(switchId, 0.f, 1.f,
                                                initialPosition, name);
}
} // namespace curvature
} // namespace dhe
