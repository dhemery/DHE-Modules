#pragma once

#include <functional>
#include <util/interval.h>
#include <util/sigmoid.h>
#include "modulated-control.h"

namespace DHE {
static constexpr auto MEDIUM_DURATION_MAX = 10.0f;
static constexpr auto DURATION_KNOB_MAX_TO_MIN_RATIO = 1000.f;
static constexpr auto MEDIUM_DURATION_MIN = MEDIUM_DURATION_MAX/DURATION_KNOB_MAX_TO_MIN_RATIO;
static constexpr auto MEDIUM_DURATION = Interval{MEDIUM_DURATION_MIN, MEDIUM_DURATION_MAX};
static constexpr auto DURATION_SCALE_STEP = 10.f;
static constexpr auto SHORT_DURATION = Interval{MEDIUM_DURATION_MIN/DURATION_SCALE_STEP, MEDIUM_DURATION_MAX/DURATION_SCALE_STEP};
static constexpr auto LONG_DURATION = Interval{MEDIUM_DURATION_MIN*DURATION_SCALE_STEP, MEDIUM_DURATION_MAX*DURATION_SCALE_STEP};
static constexpr auto DURATION_KNOB_CURVATURE = 0.8f; // Yields ~1/10 of max at center position
static constexpr auto DEFAULT_DURATION_RANGE_SWITCH = [] { return 1.f; };

struct DurationControl {
  explicit DurationControl(std::function<float()> rotation)
      : modulated_rotation{std::move(rotation)},
        range_switch{DEFAULT_DURATION_RANGE_SWITCH} {}

  DurationControl(std::function<float()> rotation,
                  std::function<float()> cv,
                  std::function<float()> range_switch = DEFAULT_DURATION_RANGE_SWITCH)
      : modulated_rotation{std::move(rotation), std::move(cv)},
        range_switch{std::move(range_switch)} {}

  float operator()() const {
    auto range = range_switch() < 0.5f ? SHORT_DURATION :
                 range_switch() < 1.5f ? MEDIUM_DURATION : LONG_DURATION;
    return range.scale(sigmoid(modulated_rotation(), DURATION_KNOB_CURVATURE));
  }

  const ModulatedControl modulated_rotation;
  const std::function<float()> range_switch;
};

}
