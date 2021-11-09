#pragma once

#include "components/sigmoid.h"
#include "signals/shapes.h"

#include "rack.hpp"

namespace dhe {
class CurvatureKnobQuantity : public rack::engine::ParamQuantity {
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    return Curvature::value(rotation);
  }

  void setDisplayValue(float curvature) override {
    auto const rotation = Curvature::rotation(curvature);
    setValue(rotation);
  }
};

} // namespace dhe
