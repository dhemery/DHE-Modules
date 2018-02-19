#pragma once

#include <functional>
#include <util/interval.h>
#include <util/sigmoid.h>
#include "modulated-control.h"

namespace DHE {

static constexpr auto DEFAULT_SHAPE_STYLE_SWITCH = [] { return 0.f; };

struct ShapeControl {
  explicit ShapeControl(std::function<float()> rotation)
      : modulated_rotation{std::move(rotation)},
        shape_switch{DEFAULT_SHAPE_STYLE_SWITCH} {}

  ShapeControl(std::function<float()> rotation,
               std::function<float()> cv,
               std::function<float()> shape_switch = DEFAULT_SHAPE_STYLE_SWITCH)
      : modulated_rotation{std::move(rotation), std::move(cv)},
        shape_switch{std::move(shape_switch)} {}

  float operator()(float x) const {
    static constexpr auto shape_curvature{-0.65f};
    auto is_sigmoid = shape_switch() > 0.5f;
    auto sign{is_sigmoid ? -1.f : 1.f};
    auto k = sign*sigmoid(BIPOLAR_NORMAL.scale(modulated_rotation()), shape_curvature);
    auto range = is_sigmoid ? BIPOLAR_NORMAL : NORMAL;
    auto shaped{sigmoid(range.scale(x), k)};
    return range.normalize(shaped);
  }

  const ModulatedControl modulated_rotation;
  const std::function<float()> shape_switch;
};

}
