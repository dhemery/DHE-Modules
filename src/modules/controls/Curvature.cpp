#include "modules/controls/Curvature.h"

#include "modules/controls/Controls.h"
#include "util/Sigmoid.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace curvature {
  static auto constexpr gentleSCurvature = 0.65F;
  static inline auto curvatureFor(float curveKnobRotation) -> float {
    // This curvature creates a gentle S curve, increasing sensitivity in the
    // middle of the input range and decreasing sensitivity toward the extremes.
    auto scaled = sigmoid::sigmoidRange.scale(curveKnobRotation);
    return sigmoid::curve(scaled, gentleSCurvature);
  }

  auto rotationToCurvature() -> std::function<float(float)> {
    return [](float rotation) -> float {
      auto const tapered = sigmoid::sTaper(rotation, gentleSCurvature);
      return sigmoid::curvatureRange.scale(tapered);
    };
  }

  auto curvatureToRotation() -> std::function<float(float)> {
    return [](float curvature) -> float {
      auto const tapered = sigmoid::curvatureRange.normalize(curvature);
      return sigmoid::sTaper(tapered, -gentleSCurvature);
    };
  }

  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int switchId)
      -> std::function<float(float)> {
    auto const rotation = knob::rotation(module, knobId, cvId);
    auto const shapeSwitch = &module->params[switchId];
    return [rotation, shapeSwitch](float input) -> float {
      auto const shapeSelection = static_cast<int>(shapeSwitch->getValue());
      auto const shape = sigmoid::shapes[shapeSelection];

      return shape->taper(input, curvatureFor(rotation()));
    };
  }

  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId)
      -> std::function<float(float)> {
    auto const rotation = knob::rotation(module, knobId, cvId, avId);
    auto const shapeSwitch = &module->params[switchId];
    return [rotation, shapeSwitch](float input) -> float {
      auto const shapeSelection = static_cast<int>(shapeSwitch->getValue());
      auto const shape = sigmoid::shapes[shapeSelection];

      return shape->taper(input, curvatureFor(rotation()));
    };
  }

  auto withFixedShape(rack::engine::Module *module, int knobId, sigmoid::Shape const *shape)
      -> std::function<float(float)> {
    auto const rotation = knob::rotation(module, knobId);
    return [rotation, shape](float input) -> float { return shape->taper(input, curvatureFor(rotation())); };
  }

  auto shape(rack::engine::Module *module, int switchId) -> std::function<sigmoid::Shape const *()> {
    auto switchParam = &module->params[switchId];
    return [switchParam]() -> sigmoid::Shape const * {
      auto const selection = static_cast<int>(switchParam->getValue());
      return sigmoid::shapes[selection];
    };
  }

  class KnobParamQuantity : public rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      static auto const curvatureTaperFor = curvature::rotationToCurvature();
      return curvatureTaperFor(getValue());
    }

    void setDisplayValue(float curvature) override {
      static auto const rotationFor = curvature::curvatureToRotation();
      setValue(rotationFor(curvature));
    }
  };

  void configKnob(rack::engine::Module *module, int knobId, std::string const &name, float initialPosition) {
    module->configParam<KnobParamQuantity>(knobId, 0.F, 1.F, initialPosition, name);
  }

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name, int initialPosition) {
    static auto const positionNames = std::array<std::string, 2>{"J", "S"};
    toggle::config<2>(module, switchId, name, positionNames, initialPosition);
  }
} // namespace curvature
} // namespace dhe
