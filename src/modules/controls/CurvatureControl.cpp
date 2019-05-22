#include <array>

#include "modules/controls/CurvatureControl.h"
#include "util/sigmoid.h"

namespace dhe {

ConstantParam CurvatureControl::constantJShapeSwitch{0.f};

auto CurvatureControl::taper(float input) -> float {
  auto const curvature = sigmoid::curvature(rotation());
  auto const shapeSelection = static_cast<int>(shapeSwitch.getValue());
  auto const shape = sigmoid::shapes()[shapeSelection];

  return shape->taper(input, curvature);
}

} // namespace dhe
