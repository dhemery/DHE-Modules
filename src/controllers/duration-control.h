#ifndef DHE_MODULES_CONTROLLERS_DURATION_CONTROL_H
#define DHE_MODULES_CONTROLLERS_DURATION_CONTROL_H

#include <functional>
#include <util/interval.h>
#include <util/sigmoid.h>

namespace DHE {

constexpr auto SHORT_DURATION = Interval{1e-4, 1.0f};
constexpr auto MEDIUM_DURATION = Interval{SHORT_DURATION.lower_bound * 10.f, SHORT_DURATION.upper_bound * 10.f};
constexpr auto LONG_DURATION = Interval{SHORT_DURATION.lower_bound * 100.f, SHORT_DURATION.upper_bound * 100.f};

struct DurationControl {
  DurationControl(std::function<float()> rotation,
                  std::function<float()> cv = [] { return 0.f; },
                  std::function<const Interval()> range = []() -> const Interval { return MEDIUM_DURATION; })
      : rotation{std::move(rotation)},
        cv{std::move(cv)},
        range{std::move(range)} {}

  float operator()() const {
    static constexpr float curvature{0.8f};

    return range().scale(sigmoid(rotation(), curvature));
  }

  const std::function<float()> rotation;
  const std::function<float()> cv;
  const std::function<const Interval()> range;
};

}

#endif
