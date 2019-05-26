#include "modules/controls/Curvature.h"

#include "modules/components/Taper.h"
#include "modules/controls/Controls.h"
#include "util/Sigmoid.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace curvature {
  /**
   * This curvature creates a gentle S taper, increasing sensitivity in the middle of the input range and decreasing
   * sensitivity toward the extremes.
   */
  static auto constexpr curveKnobTaperCurvature = -0.65F;
  static auto const curveKnobTaper = FixedSTaper{curveKnobTaperCurvature};

  class KnobParamQuantity : public rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override { return curveKnobTaper.apply(getValue()); }

    void setDisplayValue(float curvature) override { setValue(curveKnobTaper.invert(curvature)); }
  };

  void configKnob(rack::engine::Module *module, int knobId, std::string const &name, float initialRotation) {
    module->configParam<KnobParamQuantity>(knobId, 0.F, 1.F, initialRotation, name);
  }

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name, Shape initialShape) {
    static auto const positionNames = std::array<std::string, 2>{"J", "S"};
    toggle::config<2>(module, switchId, name, positionNames, initialShape);
  }

  auto knob(rack::engine::Module *module, int knobId, int cvId) -> std::function<float()> {
    auto modulatedRotation = knob::rotation(module, knobId, cvId);
    return [modulatedRotation]() -> float {
      auto const rotation = modulatedRotation();
      auto const clampedRotation = attenuverter::range.clamp(rotation);
      return curveKnobTaper.apply(clampedRotation);
    };
  }
} // namespace curvature

namespace taper {}
auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int switchId)
    -> std::function<float(float)> {
  auto curvature = curvature::knob(module, knobId, cvId);
  auto shapeSwitch = &module->params[switchId];
  return [curvature, shapeSwitch](float input) -> float {
    static auto const VariableSTaper sTaper;
    static auto const VariableJTaper jTaper;
    auto const &taper = shapeSwitch->getValue() > 0.5 ? sTaper : jTaper;
    return taper.apply(input, curvature());
  };
}
} // namespace dhe
} // namespace dhe
