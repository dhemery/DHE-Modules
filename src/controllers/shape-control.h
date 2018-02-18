#ifndef DHE_MODULES_CONTROLLERS_SHAPE_CONTROL_H
#define DHE_MODULES_CONTROLLERS_SHAPE_CONTROL_H

#include <functional>
#include <util/interval.h>
#include <util/sigmoid.h>

namespace DHE {

struct ShapeControl {
  ShapeControl(std::function<float()> rotation,
               std::function<float()> cv = [] { return 0.f; },
               std::function<int()> inflections = []() { return 0; })
      : rotation{std::move(rotation)},
        cv{std::move(cv)},
        inflections{std::move(inflections)} {}

  float operator()(float x) const {
    static constexpr auto shape_curvature{-0.65f};
    auto is_sigmoid = inflections() == 1;
    auto sign{is_sigmoid ? -1.f : 1.f};
    auto k = sign * sigmoid(BIPOLAR_NORMAL.scale(rotation()), shape_curvature);
    auto range = is_sigmoid ? BIPOLAR_NORMAL : NORMAL;
    auto shaped{sigmoid(range.scale(x), k)};
    return range.normalize(shaped);
  }

  const std::function<float()> rotation;
  const std::function<float()> cv;
  const std::function<int()> inflections;
};

}

#endif
