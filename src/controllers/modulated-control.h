#ifndef DHE_MODULES_CONTROLLERS_MODULATED_CONTROL_H
#define DHE_MODULES_CONTROLLERS_MODULATED_CONTROL_H

#include <functional>
#include <util/interval.h>

namespace DHE {

struct ModulatedControl {
  ModulatedControl(
      std::function<float()> rotation,
      std::function<float()> cv = [] { return 0.f; })
      : rotation{std::move(rotation)},
        cv{std::move(cv)} {}

  float operator()() const {
    return NORMAL.clamp(rotation() + cv() * 1e-1f);
  }

  const std::function<float()> rotation;
  const std::function<float()> cv;

};

}
#endif
