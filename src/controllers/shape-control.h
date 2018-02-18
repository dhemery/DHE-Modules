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

  float operator()() const {
    static constexpr float shape_curvature{-0.65f};
    auto factor{inflections() == 0 ? 1.f : -1.f};

    return factor * sigmoid(BIPOLAR_NORMAL.scale(rotation()), shape_curvature);
  }

  Interval range() const {
    return inflections() == 0 ? NORMAL : BIPOLAR_NORMAL;
  }

  const std::function<float()> rotation;
  const std::function<float()> cv;
  const std::function<int()> inflections;
};

}

#endif
