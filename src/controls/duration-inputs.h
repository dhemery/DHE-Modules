#pragma once

#include "common-inputs.h"
#include "components/range.h"

#include <array>

namespace dhe {

/**
 * This curvature creates a gentle inverted S taper, increasing sensitivity in
 * the middle of the knob rotation and decreasing sensitivity toward the
 * extremes.
 */
static auto constexpr duration_knob_taper_curvature = 0.8018017F;

/**
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center
 * yields a duration equal to 1/10 of the range's upper bound (to within 7
 * decimal places).
 */
static auto constexpr duration_knob_taper =
    taper::j_with_curvature(duration_knob_taper_curvature);

auto constexpr short_duration_range = Range{0.001F, 1.F};
auto constexpr medium_duration_range = Range{0.01F, 10.F};
auto constexpr long_duration_range = Range{0.1F, 100.F};

static auto constexpr duration_ranges = std::array<Range const *, 3>{
    &short_duration_range, &medium_duration_range, &long_duration_range};

static inline auto duration(float rotation, Range const &range) -> float {
  return tapered_and_scaled_rotation(rotation, duration_knob_taper, range);
}

template <typename KnobType>
auto duration(KnobType &knob, Range const &range) -> float {
  return duration(rotation_of(knob), range);
}

template <typename KnobType, typename ToggleType>
auto selectable_duration(KnobType &knob, ToggleType &toggle) -> float {
  auto const range = selected_range<3>(toggle, duration_ranges);
  return duration(rotation_of(knob), *range);
}

template <typename KnobType, typename InputType, typename ToggleType>
auto selectable_duration(KnobType &knob, InputType &cv_input,
                         ToggleType &toggle) -> float {
  auto const range = selected_range<3>(toggle, duration_ranges);
  return duration(rotation(knob, cv_input), *range);
}

} // namespace dhe
