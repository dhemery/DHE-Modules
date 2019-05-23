#include "modules/controls/Curvature.h"
#include "modules/controls/Controls.h"
#include "util/sigmoid.h"

namespace dhe {
namespace curvature {
static float constexpr curveKnobCurvature = 0.65f;

auto rotationToCurvature() -> std::function<float(float)> {
  return [](float rotation) -> float { return sigmoid::curvature(rotation); };
}

auto curvatureToRotation() -> std::function<float(float)> {
  return [](float taper) -> float {
    return sigmoid::s_taper(taper, -curveKnobCurvature);
  };
}

auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId,
                         int switchId) -> std::function<float(float)> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId, cvId);
  auto const shapeSwitch = &module->params[switchId];
  return [rotation, shapeSwitch](float input) -> float {
    auto const curvature = sigmoid::curvature(rotation());
    auto const shapeSelection = static_cast<int>(shapeSwitch->getValue());
    auto const shape = sigmoid::shapes[shapeSelection];

    return shape->taper(input, curvature);
  };
}

auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId,
                         int avId, int switchId)
    -> std::function<float(float)> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId, cvId, avId);
  auto const shapeSwitch = &module->params[switchId];
  return [rotation, shapeSwitch](float input) -> float {
    auto const curvature = sigmoid::curvature(rotation());
    auto const shapeSelection = static_cast<int>(shapeSwitch->getValue());
    auto const shape = sigmoid::shapes[shapeSelection];

    return shape->taper(input, curvature);
  };
}

auto withFixedShape(rack::engine::Module *module, int knobId,
                    sigmoid::Shape const *shape)
    -> std::function<float(float)> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId);
  return [rotation, shape](float input) -> float {
    auto const curvature = sigmoid::curvature(rotation());

    return shape->taper(input, curvature);
  };
}

auto shape(rack::engine::Module *module, int switchId)
    -> std::function<sigmoid::Shape const *()> {
  auto switchParam = &module->params[switchId];
  return [switchParam]() -> sigmoid::Shape const * {
    auto const selection = static_cast<int>(switchParam->getValue());
    return sigmoid::shapes[selection];
  };
}
} // namespace curvature
} // namespace dhe
