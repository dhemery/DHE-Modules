#ifndef DHE_MODULES_CONTROLLERS_MODULATED_CONTROL_H
#define DHE_MODULES_CONTROLLERS_MODULATED_CONTROL_H

#include <functional>
#include <util/interval.h>

namespace DHE {


struct ModulatedControl {
  static constexpr auto DEFAULT_CV = 0.f;
  static constexpr auto CV_NORMALIZATION_FACTOR = 1e-1f;

  ModulatedControl(std::function<float()> normalized_input,
                   std::function<float()> cv = [] { return DEFAULT_CV; })
      : rotation{std::move(normalized_input)},
        cv{std::move(cv)} {}

  float operator()() const {
    return NORMAL.clamp(rotation() + cv()*CV_NORMALIZATION_FACTOR);
  }

  const std::function<float()> rotation;
  const std::function<float()> cv;

};

}
#endif
