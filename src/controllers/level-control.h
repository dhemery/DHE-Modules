#ifndef DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H
#define DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H

#include <util/interval.h>

namespace DHE {

struct LevelControl {
  LevelControl(std::function<float()> rotation,
               std::function<float()> cv = [] { return 0.f; },
               std::function<bool()> unipolar_range = [] { return true; })
      : rotation{std::move(rotation)},
        cv{std::move(cv)},
        unipolar{std::move(unipolar_range)} {}

  float operator()() const {
    auto range = unipolar() ? UNIPOLAR_CV : BIPOLAR_CV;
    return range.scale(rotation()) + cv();
  }

  const std::function<float()> rotation;
  const std::function<float()> cv;
  const std::function<bool()> unipolar;
};

}

#endif
