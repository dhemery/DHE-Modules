#ifndef DHE_MODULES_CONTROLLERS_DURATION_CONTROL_H
#define DHE_MODULES_CONTROLLERS_DURATION_CONTROL_H

#include <functional>
#include <util/interval.h>
#include <util/sigmoid.h>
#include <util/modulation.h>

namespace DHE {

constexpr auto SHORT_DURATION = Interval{1e-4, 1.0f};
constexpr auto MEDIUM_DURATION = Interval{SHORT_DURATION.lower_bound * 10.f, SHORT_DURATION.upper_bound * 10.f};
constexpr auto LONG_DURATION = Interval{SHORT_DURATION.lower_bound * 100.f, SHORT_DURATION.upper_bound * 100.f};

struct DurationControl {
  DurationControl(std::function<float()> rotation,
                  std::function<float()> cv = [] { return 0.f; },
                  std::function<float()> range_switch = [] { return 1.f; })
      : rotation{std::move(rotation)},
        cv{std::move(cv)},
        range_switch{std::move(range_switch)} {}

  float operator()() const {
    static constexpr auto curvature{0.8f};
    auto range = range_switch() < 0.5f ? SHORT_DURATION :
                 range_switch() < 1.5f ? MEDIUM_DURATION : LONG_DURATION;
    return range.scale(sigmoid(modulated(rotation(), cv()), curvature));
  }

  const std::function<float()> rotation;
  const std::function<float()> cv;
  const std::function<float()> range_switch;
};

}

#endif
