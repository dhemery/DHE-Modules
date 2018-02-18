#ifndef DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H
#define DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H

#include <functional>
#include <util/interval.h>
#include "modulated-control.h"

namespace DHE {

struct LevelControl {
  LevelControl(std::function<float()> rotation,
               std::function<float()> cv = [] { return 0.f; },
               std::function<float()> range_switch = [] { return 1.f; })
      : modulated_level{std::move(rotation), std::move(cv)},
        range_switch{std::move(range_switch)} {}

  float operator()() const {
    auto range = range_switch() < 0.5 ? BIPOLAR_CV : UNIPOLAR_CV;
    return range.scale(modulated_level());
  }

  const ModulatedControl modulated_level;
  const std::function<float()> range_switch;
};

}

#endif
