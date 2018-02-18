#ifndef DHE_MODULES_CONTROLLERS_SHAPE_CONTROL_H
#define DHE_MODULES_CONTROLLERS_SHAPE_CONTROL_H

#include <functional>
#include <util/interval.h>
#include <util/sigmoid.h>
#include "modulated-control.h"

namespace DHE {

struct ShapeControl {
  ShapeControl(std::function<float()> rotation,
               std::function<float()> cv = [] { return 0.f; },
               std::function<float()> shape_switch = []() { return 0.f; })
      : modulated_shape_knob{std::move(rotation), std::move(cv)},
        shape_switch{std::move(shape_switch)} {}

  float operator()(float x) const {
    static constexpr auto shape_curvature{-0.65f};
    auto is_sigmoid = shape_switch() > 0.5f;
    auto sign{is_sigmoid ? -1.f : 1.f};
    auto k = sign * sigmoid(BIPOLAR_NORMAL.scale(modulated_shape_knob()), shape_curvature);
    auto range = is_sigmoid ? BIPOLAR_NORMAL : NORMAL;
    auto shaped{sigmoid(range.scale(x), k)};
    return range.normalize(shaped);
  }

  const ModulatedControl modulated_shape_knob;
  const std::function<float()> shape_switch;
};

}

#endif
