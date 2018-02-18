#ifndef DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H
#define DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H

#include <functional>
#include <util/interval.h>

namespace DHE {

struct LevelControl {
  LevelControl(std::function<float()> rotation,
               std::function<float()> cv = [] { return 0.f; },
               std::function<float()> range_switch = [] { return 1.f; })
      : rotation{std::move(rotation)},
        cv{std::move(cv)},
        range_switch{std::move(range_switch)} {}

  float operator()() const {
    auto range = range_switch() < 0.5 ? BIPOLAR_CV : UNIPOLAR_CV;
    return range.scale(rotation()) + cv();
  }

  const std::function<float()> rotation;
  const std::function<float()> cv;
  const std::function<float()> range_switch;
};

}

#endif
