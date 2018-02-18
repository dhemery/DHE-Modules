#ifndef DHE_MODULES_CONTROLLERS_DURATION_CONTROL_H
#define DHE_MODULES_CONTROLLERS_DURATION_CONTROL_H

#include <functional>
#include <util/interval.h>
#include <util/sigmoid.h>
#include "modulated-control.h"

namespace DHE {

constexpr auto SHORT_DURATION = Interval{1e-4, 1.0f};
constexpr auto MEDIUM_DURATION = Interval{SHORT_DURATION.lower_bound*10.f, SHORT_DURATION.upper_bound*10.f};
constexpr auto LONG_DURATION = Interval{SHORT_DURATION.lower_bound*100.f, SHORT_DURATION.upper_bound*100.f};

struct DurationControl {
  DurationControl(std::function<float()> rotation,
                  std::function<float()> cv = [] { return 0.f; },
                  std::function<float()> range_switch = [] { return 1.f; })
      : modulated_duration{std::move(rotation), std::move(cv)},
        range_switch{std::move(range_switch)} {}

  float operator()() const {
    static constexpr auto curvature{0.8f};
    auto range = range_switch() < 0.5f ? SHORT_DURATION :
                 range_switch() < 1.5f ? MEDIUM_DURATION : LONG_DURATION;
    return range.scale(sigmoid(modulated_duration(), curvature));
  }

  const ModulatedControl modulated_duration;
  const std::function<float()> range_switch;
};

}

#endif
