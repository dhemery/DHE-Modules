#ifndef DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H
#define DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H

#include <functional>
#include <util/interval.h>

namespace DHE {

struct LevelControl {
  LevelControl(std::function<float()> rotation,
               std::function<float()> cv = [] { return 0.f; },
               std::function<const Interval()> range = [] { return UNIPOLAR_CV; })
      : rotation{std::move(rotation)},
        cv{std::move(cv)},
        range{std::move(range)} {}

  float operator()() const {
    return range().scale(rotation()) + cv();
  }

  const std::function<float()> rotation;
  const std::function<float()> cv;
  const std::function<const Interval()> range;
};

}

#endif
