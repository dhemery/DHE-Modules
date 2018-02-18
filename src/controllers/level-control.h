#ifndef DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H
#define DHE_MODULES_CONTROLLERS_LEVEL_CONTROL_H

#include <functional>
#include <util/interval.h>
#include "modulated-control.h"

namespace DHE {
static constexpr auto DEFAULT_LEVEL_RANGE_SWITCH = [] { return 1.f; };

struct LevelControl {
  explicit LevelControl(std::function<float()> rotation)
      : modulated_rotation{std::move(rotation)},
        range_switch{DEFAULT_LEVEL_RANGE_SWITCH} {}

  LevelControl(std::function<float()> rotation,
               std::function<float()> cv,
               std::function<float()> range_switch = DEFAULT_LEVEL_RANGE_SWITCH)
      : modulated_rotation{std::move(rotation), std::move(cv)},
        range_switch{std::move(range_switch)} {}

  float operator()() const {
    auto range = range_switch() < 0.5 ? BIPOLAR_CV : UNIPOLAR_CV;
    return range.scale(modulated_rotation());
  }

  const ModulatedControl modulated_rotation;
  const std::function<float()> range_switch;
};

}

#endif
