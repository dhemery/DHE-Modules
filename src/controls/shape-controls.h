#pragma once

#include "signals/curvature-signals.h"

#include "rack.hpp"

#include <string>

namespace dhe {
struct CurvatureKnob {
  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return Curvature::scale(getValue());
    }

    void setDisplayValue(float display_value) override {
      setValue(Curvature::normalize(display_value));
    }
  };

  static auto config(rack::engine::Module *module, int id,
                     std::string const &name, float default_curvature = 0.F)
      -> Quantity * {
    auto const default_rotation = Curvature::normalize(default_curvature);
    auto *q =
        module->configParam<Quantity>(id, 0.F, 1.F, default_rotation, name);
    return q;
  }
};
} // namespace dhe